#include "ECS.h"
#include "Utils.h"

namespace Core::Ecs
{

	void ECS::RemoveEntity(EntityID entity)
	{
		CORE_PROFILE_FUNCTION();

		auto it = std::find_if(m_EntityMasks.begin(), m_EntityMasks.end(),
			[entity](const EntityDesc& e) { return e.id == entity; });

		if (it != m_EntityMasks.end())
		{
			// Remove the entity from the mask list
			m_EntityMasks.erase(it);

			// Remove all components associated with the entity
			for (auto& [typeId, componentStorage] : m_Components)
			{
				// Find the component for this entity
				auto compIt = componentStorage.find(entity);
				if (compIt != componentStorage.end())
				{
					// Free dynamically allocated memory
					delete static_cast<char*>(compIt->second); // Ensure all components are allocated with new!
					componentStorage.erase(compIt);            // Remove from storage
				}
			}
		}
	}

	ECS::EntityID ECS::CreateEntityWithID(ECS::EntityID id)
	{
		CORE_PROFILE_FUNCTION();

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