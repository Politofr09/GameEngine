#include "Project.h"
#include "Instrumentor.h"

#include <fstream>

namespace Core
{

	void Project::Serialize(const std::string& path)
	{
		CORE_PROFILE_FUNCTION();

		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Project" << YAML::Value << YAML::BeginMap;
		out << YAML::Key << "Name" << YAML::Value << m_Name;
		out << YAML::Key << "Author" << YAML::Value << m_Author;
		out << YAML::Key << "ID" << YAML::Value << id;
		out << YAML::Key << "Scene" << YAML::Value << m_ScenePath;
		out << YAML::Key << "AssetDef" << YAML::Value << m_AssetRegistryPath;
		out << YAML::EndMap;

		std::ofstream fout(path);
		fout << out.c_str();
		fout.close();

		m_Scene.Serialize(m_ScenePath);
		m_AssetRegistry.Serialize(m_AssetRegistryPath);
	}

	bool Project::Deserialize(const std::string& path)
	{
		CORE_PROFILE_FUNCTION();

		YAML::Node data;
		try
		{
			data = YAML::LoadFile(path);
		}
		catch (YAML::ParserException e)
		{
			LOG_ERROR("Failed to load project file", e.what());
			return false;
		}

		m_Name = data["Project"]["Name"].as<std::string>();
		m_Author = data["Project"]["Author"].as<std::string>();
		id = data["Project"]["ID"].as<uint64_t>();

		if (auto scenePathNode = data["Project"]["Scene"])
		{
			m_ScenePath = scenePathNode.as<std::string>();
			m_Scene.Deserialize(m_ScenePath);
		}

		if (auto assetRegistryPathNode = data["Project"]["AssetDef"])
		{
			m_AssetRegistryPath = assetRegistryPathNode.as<std::string>();
			m_AssetRegistry.Deserialize(m_AssetRegistryPath);
		}
	}

}
