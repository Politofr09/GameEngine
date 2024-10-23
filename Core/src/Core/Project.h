#pragma once

#include "Utils.h"
#include "UUID.h"
#include "AssetRegistry.h"
#include "Scene.h"

#include <string>

namespace Core
{

	// A class for managing the state of the application
	class Project
	{
	public:
		Project() = default;
		Project(const std::string& name, const std::string& author)
		{
			m_Name = name;
			m_Author = author;
		}

		std::string m_Name;
		std::string m_Author;
		UUID id;

		void Serialize(const std::string& path);
		bool Deserialize(const std::string& path);

		AssetRegistry& GetRegistry() { return m_AssetRegistry; }
		Scene& GetScene() { return m_Scene; }

	private:
		AssetRegistry m_AssetRegistry;
		std::string m_AssetRegistryPath = "";

		// TODO make senes a vector so we can have more than one scene in one project
		Scene m_Scene;
		std::string m_ScenePath = "";
	};

}