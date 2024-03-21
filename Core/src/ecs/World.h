#include <vector>
#include <unordered_map>
#include <typeindex>
#include <memory>
#include <thread>
#include <algorithm>
#include "Components.h"
#include "ComponentStorage.h"
#include "System.h"

#pragma once

class World
{
private:
	using Entity = std::size_t;

	std::vector<Entity> _entities;
	Entity nextEntity = -1;

	std::unordered_map<std::type_index, std::unique_ptr<ComponentStorageBase>> _componentStorages;

	std::vector<System*> _systems;

public:
	World() {}

	Entity CreateEntity()
	{
		Entity entity = nextEntity++;
		_entities.push_back(entity);
		return entity;
	}

	void DestroyEntity(Entity entity)
	{
		auto it = std::find(_entities.begin(), _entities.end(), entity);
		if (it != _entities.end())
		{
			_entities.erase(it);
		}
	}

	inline Entity GetEntityCount()
	{
		return _entities.size();
	}
	
	// Register a component type with its corresponding storage
	template<typename T>
	void RegisterComponentType() {
		_componentStorages[typeid(T)] = std::make_unique<ComponentStorage<T>>();
	}

	// Get the ComponentStorageBase associated with the given component type
	template<typename T>
	ComponentStorage<T>& GetComponentStorage() {
		auto it = _componentStorages.find(typeid(T));
		if (it == _componentStorages.end()) {
			throw std::runtime_error("Component type not registered");
		}
		return dynamic_cast<ComponentStorage<T>&>(*it->second);
	}

	template<typename T>
	T& GetComponent(Entity entity)
	{
		ComponentStorage<T>& storage = GetComponentStorage<T>();
		return storage.GetComponent(entity);
	}

	template<typename T>
	void AddComponent(Entity entity, T& component)
	{
		// Check if component storage exists, if not, create one
		if (_componentStorages.find(typeid(T)) == _componentStorages.end()) {
			RegisterComponentType<T>();
		}

		// Retrieve the component storage and add the component
		ComponentStorage<T>& storage = GetComponentStorage<T>();
		storage.AddComponent(entity, component);
	}

	template<typename T>
	bool HasComponent(Entity entity)
	{
		ComponentStorage<T>& storage = GetComponentStorage<T>();
		return storage.HasEntity(entity);
	}

	template<typename T>
	void RemoveComponent(Entity entity)
	{
		ComponentStorage<T>& storage = GetComponentStorage<T>();
		storage.RemoveComponent(entity);
	}

	template<typename... Components>
	std::vector<Entity> GetEntitiesWith()
	{
		std::vector<Entity> result;

		// Iterate over all entities and check if they have all specified component types
		for (Entity entity : _entities)
		{
			if ((HasComponent<Components>(entity) && ...))
			{
				result.push_back(entity);
			}
		}

		return result;
	}

	void AddSystem(System* sys)
	{
		_systems.push_back(sys);
	}

	void Tick()
	{
		// Sort systems based on their priority
		std::sort(_systems.begin(), _systems.end(), [](System* a, System* b) {
			return a->GetPriority() < b->GetPriority();
			});

		// Find systems with the same priority and run them concurrently
		int prevPriority = -1;
		std::vector<std::thread> threads;
		for (System* system : _systems) {
			if (system->GetPriority() != prevPriority) {
				// Wait for previous threads to finish before starting new ones
				for (std::thread& t : threads) {
					t.join();
				}
				threads.clear();
				prevPriority = system->GetPriority();
			}
			threads.push_back(std::thread(&System::Tick, system)); // Start a new thread for each system
		}

		// Wait for all threads to finish
		for (std::thread& t : threads) {
			t.join();
		}
	}

	void RemoveSystem(System* sys)
	{
		_systems.erase(std::find(_systems.begin(), _systems.end(), sys));
	}

	~World()
	{
		_entities.clear();

		_componentStorages.clear();

		for (auto sys : _systems)
		{
			delete sys;
		}

		_systems.clear();
	}
};