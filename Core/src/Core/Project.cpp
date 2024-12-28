#include "Project.h"
#include "Instrumentor.h"
#include "UUID.h"

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
		out << YAML::Key << "AssetDirectory" << YAML::Value << m_AssetDirectory;
		out << YAML::EndMap;

		std::ofstream fout(path);
		fout << out.c_str();
		fout.close();

		m_Scene.Serialize(m_ScenePath);
		m_AssetRegistry.Serialize();
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
			LOG_ERROR(std::string("Failed to load project file ") + std::string(e.what()));
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

		if (auto assetRegistryDirectoryNode = data["Project"]["AssetDirectory"])
		{
			m_AssetDirectory = assetRegistryDirectoryNode.as<std::string>();
			m_AssetRegistry.SetAssetDirectory(m_AssetDirectory);
			m_AssetRegistry.Deserialize();
		}
		return true;
	}

}
