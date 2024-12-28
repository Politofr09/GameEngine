#pragma once

#include "Core/UUID.h"
#include "Core/Utils.h"
#include "Common.h"

#include <bitset>
#include <typeindex>
#include <typeinfo>
#include <type_traits>
#include <unordered_map>
#include <functional>

namespace Core::Ecs
{

// Max component types
#ifndef MAX_COMPONENTS
	#define MAX_COMPONENTS 32
#endif

	// Component-mask ecs with fixed size
	class ECS
	{
	public:
		// TODO: We can do better.
		ECS()
		{
			RegisterComponent<NameComponent>();
			RegisterComponent<TransformComponent>();
			RegisterComponent<ModelComponent>();
			RegisterComponent<SpriteComponent>();
		}

		using EntityID = uint64_t;
		typedef std::bitset<MAX_COMPONENTS> ComponentMask;

		struct EntityDesc
		{
			EntityID id = UUID();
			ComponentMask mask;
		};

		using System = std::function<void(ECS&, float)>;

		template<typename... Cs>
		class View {
		public:
			View(ECS& ecs)
				: m_ECS(ecs) {}

			class Iterator {
			public:
				Iterator(ECS& ecs, size_t index)
					: m_ECS(ecs), m_Index(index) {
					AdvanceToNextValid();
				}

				Iterator& operator++() {
					++m_Index;
					AdvanceToNextValid();
					return *this;
				}

				bool operator!=(const Iterator& other) const {
					return m_Index != other.m_Index;
				}

				EntityID operator*() const {
					return m_ECS.m_EntityMasks[m_Index].id;
				}

			private:
				void AdvanceToNextValid() {
					while (m_Index < m_ECS.m_EntityMasks.size() && !HasComponents()) {
						++m_Index;
					}
				}

				bool HasComponents() const {
					const auto& entityMask = m_ECS.m_EntityMasks[m_Index].mask;
					return (entityMask & m_ECS.GetComponentMask<Cs...>()) == m_ECS.GetComponentMask<Cs...>();
				}

				ECS& m_ECS;
				size_t m_Index;
			};

			Iterator begin() {
				return Iterator(m_ECS, 0);
			}

			Iterator end() {
				return Iterator(m_ECS, m_ECS.m_EntityMasks.size());
			}

		private:
			ECS& m_ECS;
		};
	public:
		template<typename... ECS>
		View<ECS...>& GetView() 
		{
			return *new View<ECS...>(*this);
		}

		template <typename T>
		void RegisterComponent()
		{
			const std::type_index typeID = typeid(T);

			if (m_ComponentIndices.find(typeID) == m_ComponentIndices.end())
			{
				size_t newIndex = m_ComponentIndices.size();
				if (newIndex >= MAX_COMPONENTS)
				{
					LOG_ERROR("Exceeded maximum number of component types");
				}
				m_ComponentIndices[typeID] = newIndex;
			}
		}

		EntityID CreateEntityWithID(EntityID id);
		EntityID CreateEntity();

		void RemoveEntity(EntityID entity);

		template<typename T>
		void RemoveComponent(EntityID entity)
		{
			const size_t componentIndex = GetComponentIndex<T>();

			auto it = std::find_if(m_EntityMasks.begin(), m_EntityMasks.end(),
				[entity](const EntityDesc& e) { return e.id == entity; });

			if (it != m_EntityMasks.end())
			{
				// Clear the component bit in the entity's mask
				it->mask.reset(componentIndex);

				// Remove the component from the entity's storage
				delete static_cast<T*>(m_Components[typeid(T)][entity]);
				m_Components[typeid(T)].erase(entity);
			}
			else
			{
				LOG_ERROR("Entity not found");
			}
		}

		template <typename T>
		void AddComponent(EntityID entity, T& component)
		{
			size_t componentIndex = GetComponentIndex<T>();

			// Find the entity in the entity masks
			auto it = std::find_if(m_EntityMasks.begin(), m_EntityMasks.end(),
				[entity](const EntityDesc& e) { return e.id == entity; });

			if (it != m_EntityMasks.end())
			{
				// Set the component bit in the entity's mask
				it->mask.set(componentIndex);

				// Add the component to the entity's storage
				m_Components[typeid(T)][entity] = new T(component);
			}
			else
			{
				LOG_ERROR("Entity not found.");
			}
		}

		template <typename T>
		void AddComponent(EntityID entity, const T& component)
		{
			size_t componentIndex = GetComponentIndex<T>();

			// Find the entity in the entity masks
			auto it = std::find_if(m_EntityMasks.begin(), m_EntityMasks.end(),
				[entity](const EntityDesc& e) { return e.id == entity; });

			if (it != m_EntityMasks.end())
			{
				// Set the component bit in the entity's mask
				it->mask.set(componentIndex);

				// Add the component to the entity's storage
				m_Components[typeid(T)][entity] = new T(component);
			}
			else
			{
				LOG_ERROR("Entity not found.");
			}
		}

		template <typename T>
		bool HasComponent(EntityID entity)
		{
			const size_t componentIndex = GetComponentIndex<T>();

			// Find the entity in the entity masks
			auto it = std::find_if(m_EntityMasks.begin(), m_EntityMasks.end(),
				[entity](const EntityDesc& e) { return e.id == entity; });

			if (it != m_EntityMasks.end() && it->mask.test(componentIndex))
			{
				return true;
			}
			return false;
		}

		template<typename T>
		T& GetComponent(EntityID entity)
		{
			const size_t componentIndex = GetComponentIndex<T>();

			auto it = std::find_if(m_EntityMasks.begin(), m_EntityMasks.end(),
				[entity](const EntityDesc& e) { return e.id == entity; });

			ASSERT(it != m_EntityMasks.end() && it->mask.test(componentIndex));
			return *static_cast<T*>(m_Components[typeid(T)][entity]);
		}

		template<typename... Cs>
		ComponentMask GetComponentMask()
		{
			ComponentMask mask;
			((mask.set(GetComponentIndex<Cs>())), ...);
			return mask;
		}

		void AddSystem(System system);

		// Trait to check if T has a Tick(ECS&, float) method
		template<typename T, typename = void>
		struct has_tick_method : std::false_type {};

		template<typename T>
		struct has_tick_method<T, std::void_t<decltype(
			std::declval<T>().Tick(std::declval<ECS&>(), std::declval<float>())
			)>> : std::true_type {};

		// Trait to check if T has an OnStart() method
		template<typename T, typename = void>
		struct has_onstart_method : std::false_type {};

		template<typename T>
		struct has_onstart_method<T, std::void_t<decltype(
			std::declval<T>().OnStart(std::declval<ECS&>())
			)>> : std::true_type {};

		template<typename T>
		void AddSystem(T* system)
		{
			// Ensure T has required methods
			static_assert(has_tick_method<T>::value, "System does not have required Tick(ECS&, float) method!");
			static_assert(has_onstart_method<T>::value, "System does not have required OnStart(ECS&) method!");
			
			system->OnStart(*this);

			m_Systems.push_back(std::bind(&T::Tick, system, std::placeholders::_1, std::placeholders::_2));
		}

		void Tick(float dt);

		template<typename Func>
		void each(Func&& func) 
		{
			for (const auto& entityDesc : m_EntityMasks) 
			{
				func(entityDesc.id);
			}
		}

	private:
		template<typename T>
		size_t GetComponentIndex()
		{
			const std::type_index typeId = typeid(T);

			if (m_ComponentIndices.find(typeId) == m_ComponentIndices.end())
			{
				LOG_ERROR("Component type not registered. Did you call RegisterComponent?");
			}

			return m_ComponentIndices[typeId];
		}

	private:
		/* Maps each component type to a unique index bitmask
		* PositionComponent -> index 0
		* VelocityComponent -> index 1
		* ColorComponent	-> index 2
		* ...
		* 
		* Component mask:
		* [0],		    [1],		   [0], ...
		* ^			    ^			   ^
		* Position(no)  Velocity(yes)  Color(no) ...
		****************************************************/
		std::unordered_map<std::type_index, size_t> m_ComponentIndices;

		// Entity mask storage
		std::vector<EntityDesc> m_EntityMasks;

		// Component storage
		std::unordered_map<std::type_index, std::unordered_map<EntityID, void*>> m_Components;

		std::vector<System> m_Systems;
	};
	
}