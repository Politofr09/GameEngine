#include <Ecs/ECS.h>

#pragma once

namespace Core::Ecs
{

	// Higher-level entity abstraction
	class Entity
	{
	public:
		Entity(UUID id, ECS& ecs, const std::string& name = "Unnamed Entity")
			: m_EntityID(id), m_ECS(ecs), m_Name(name) {}

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

		void SetName(const std::string& name) { m_Name = name; }
		const std::string& GetName() { return m_Name; }

		UUID GetID() { return m_EntityID; }

	private:
		ECS& m_ECS;
		UUID m_EntityID;
		std::string m_Name;
	};

}