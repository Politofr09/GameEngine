#include "ECS.h"
#include "Utils.h"

namespace Core::Ecs
{

	void ECS::RemoveEntity(EntityID entity)
	{
		auto it = std::find_if(m_EntityMasks.begin(), m_EntityMasks.end(),
			[entity](const EntityDesc& e) { return e.id == entity; });

		if (it != m_EntityMasks.end())
		{
			// Remove components for the entity
			for (const auto& [typeId, componentStorage] : m_Components)
			{
				auto compIt = componentStorage.find(entity);
				if (compIt != componentStorage.end())
				{
					delete static_cast<char*>(compIt->second); // Free the component's memory
					m_Components[typeId].erase(entity);        // Remove from storage
				}
			}
			m_EntityMasks.erase(it);
		}
	}

	ECS::EntityID ECS::CreateEntityWithID(ECS::EntityID id)
	{

		// Check if the ID already exists
		auto it = std::find_if(m_EntityMasks.begin(), m_EntityMasks.end(),
			[id](const EntityDesc& e) { return e.id == id; });

		if (it != m_EntityMasks.end())
		{
			LOG_ERROR("Entity with the given ID already exists!");
			LOG_INFO("Entity will have randomly generated UUID.");
			id = UUID(); // Generate random uint64_t with constructor
		}

		EntityDesc newEntity;
		newEntity.id = id;
		newEntity.mask.reset();
		m_EntityMasks.push_back(newEntity);

		return id;
	}

	ECS::EntityID ECS::CreateEntity()
	{
		EntityID newId = UUID();
		return CreateEntityWithID(newId);
	}

	void ECS::AddSystem(System system)
	{
		m_Systems.push_back(system);
	}

	void ECS::Tick(float dt)
	{
		for (auto& system : m_Systems)
		{
			system(*this, dt);
		}
	}

}