#include <Ecs/ECS.h>

#pragma once

namespace Core::Ecs
{

	// Higher-level entity abstraction
	class Entity
	{
	public:
		Entity(UUID id, ECS& ecs)
			: m_EntityID(id), m_ECS(ecs) {}

		template <typename T>
		void AddComponent(T& component)
		{
			m_ECS.AddComponent<T>(m_EntityID, component);
		}

		template<typename T>
		T& GetComponent()
		{
			return m_ECS.GetComponent<T>(m_EntityID);
		}

		template <typename T>
		void RemoveComponent()
		{
			m_ECS.RemoveComponent<T>(m_EntityID);
		}

		template <typename T>
		bool HasComponent()
		{
			return m_ECS.HasComponent<T>(m_EntityID);
		}

		void SetName(const std::string& name) { GetComponent<NameComponent>().Name = name; }
		const std::string& GetName() { return GetComponent<NameComponent>().Name; }

		UUID GetID() { return m_EntityID; }

	private:
		ECS& m_ECS;
		UUID m_EntityID;
	};

}