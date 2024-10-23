#include "Scene.h"

#include "Utils.h"
#include "Ecs/Common.h"
#include "Ecs/VisualComponents.h"
#include <fstream>

namespace Core
{

	using namespace Ecs;

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec2& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
		return out;
	}

	glm::vec3 YAMLToVec3(const YAML::Node& node)
	{
		if (node.IsSequence() && node.size() == 3)
		{
			return glm::vec3(node[0].as<float>(), node[1].as<float>(), node[2].as<float>());
		}
		glm::vec3(0.0f);
	}

	Entity Scene::CreateEntity(const std::string& name)
	{
		Entity entity(m_Registry.CreateEntity(), m_Registry);
		entity.AddComponent<NameComponent>(NameComponent{ name });

		return entity;
	}


	void Scene::Serialize(const std::string& path)
	{
		YAML::Emitter out;
		out << YAML::BeginMap; // Root map
		{
			out << YAML::Key << "Scene" << YAML::Value << "Untitled";  // Name of the scene
			out << YAML::Key << "ID" << YAML::Value << m_ID;  // Scene ID

			out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;  // Begin sequence for entities
			m_Registry.each([&](ECS::EntityID e)
				{
					Entity fullEntity(e, m_Registry);
					SerializeEntity(out, fullEntity);  // Call helper to serialize each entity
				});
			out << YAML::EndSeq; // End sequence for entities
		}
		out << YAML::EndMap; // End root map

		// Write the result to file
		std::ofstream fout(path);
		fout << out.c_str();
		fout.close();
	}

	bool Scene::Deserialize(const std::string& path)
	{
		YAML::Node data;
		try
		{
			data = YAML::LoadFile(path);
		}
		catch (YAML::Exception e)
		{
			LOG_ERROR("Failed to load scene file", e.what());
			return false;
		}

		YAML::Node scene = data["Scene"];
		if (!scene)
		{
			LOG_ERROR("Scene file is invalid or corrupted.");
			return false;
		}

		m_ID = data["ID"].as<uint64_t>(UUID());

		// TODO: Deserialize m_Name when added LOL

		YAML::Node entities = data["Entities"];
		if (!entities)
		{
			LOG_ERROR("Scene file is invalid or corrupted.");
			return false;
		}

		for (auto entityNode: entities)
		{
			DeserializeEntity(entityNode);
		}

		return true;
	}

	void Scene::SerializeEntity(YAML::Emitter& out, Entity& entity)
	{
		// TODO: Serialize 'em all
		out << YAML::BeginMap;
		out << YAML::Key << "Entity" << YAML::Value << entity.GetID();

		if (entity.HasComponent<NameComponent>())
		{
			out << YAML::Key << "NameComponent" << YAML::Value << entity.GetName();
		}

		if (entity.HasComponent<TransformComponent>())
		{
			out << YAML::Key << "TransformComponent";
			out << YAML::BeginMap;
			{
				auto& t = entity.GetComponent<TransformComponent>();
				out << YAML::Key << "Position" << YAML::Value << t.Position;
				out << YAML::Key << "Rotation" << YAML::Value << t.Rotation;
				out << YAML::Key << "Scale" << YAML::Value << t.Scale;
			}
			out << YAML::EndMap;
		}

		if (entity.HasComponent<ModelComponent>())
		{
			out << YAML::Key << "ModelComponent";
			out << YAML::BeginMap;
			{
				auto& m = entity.GetComponent<ModelComponent>();
				out << YAML::Key << "ModelHandle" << YAML::Value << m.ModelHandle;
			}
			out << YAML::EndMap;
		}

		if (entity.HasComponent<TextureComponent>())
		{
			out << YAML::Key << "TextureComponent";
			out << YAML::BeginMap;
			{
				auto& t = entity.GetComponent<TextureComponent>();
				out << YAML::Key << "ModelHandle" << YAML::Value << t.TextureHandle;
			}
			out << YAML::EndMap;
		}

		out << YAML::EndMap;
	}

	bool Scene::DeserializeEntity(YAML::Node& node)
	{
		// TODO: Deserialize 'em all
		uint64_t entityID = node["Entity"].as<uint64_t>();
		Entity entity(m_Registry.CreateEntityWithID(entityID), m_Registry);

		std::string name = "Unnamed Entity";
		if (auto nameNode = node["NameComponent"])
		{
			name = nameNode.as<std::string>();
			entity.AddComponent<NameComponent>(NameComponent{ name });
		}

		if (auto transformNode = node["TransformComponent"])
		{
			TransformComponent t{};

			t.Position = YAMLToVec3(transformNode["Position"]);
			t.Rotation = YAMLToVec3(transformNode["Rotation"]);
			t.Scale = YAMLToVec3(transformNode["Scale"]);

			entity.AddComponent<TransformComponent>(t);
		}

		if (auto modelNode = node["ModelComponent"])
		{
			ModelComponent m{};

			m.ModelHandle = modelNode["ModelHandle"].as<uint64_t>();

			entity.AddComponent<ModelComponent>(m);
		}

		if (auto textureNode = node["TextureComponent"])
		{
			TextureComponent t{};

			t.TextureHandle = textureNode["TextureHandle"].as<uint64_t>();

			entity.AddComponent<TextureComponent>(t);
		}

		return true;
	}

}