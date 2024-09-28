#include "ECS.h"

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

	ECS::EntityID ECS::CreateEntity()
	{
		EntityDesc entity;
		m_EntityMasks.push_back(entity);
		return entity.id;
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