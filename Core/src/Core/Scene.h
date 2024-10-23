#pragma once

#include "Ecs/Entity.h"
#include "Ecs/ECS.h"
#include <yaml-cpp/yaml.h>

namespace Core
{

	class Scene
	{
	public:
		Scene() = default;

		Ecs::Entity CreateEntity(const std::string& name = "Unnamed entity");

		void Serialize(const std::string& path);
		bool Deserialize(const std::string& path);

		static void SerializeEntity(YAML::Emitter& out, Ecs::Entity& entity);
		bool DeserializeEntity(YAML::Node& node);

		Ecs::ECS& GetRegistry() { return m_Registry; }

	private:
		Ecs::ECS m_Registry;
		UUID m_ID;
	};

}