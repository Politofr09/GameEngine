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

		AssetRegistry registry; // TODO: Serialize this
		Scene scene;			// TODO: Serialize this

		friend YAML::Emitter& operator<<(YAML::Emitter& out, const Project& p)
		{
			out << YAML::BeginMap;
			out << YAML::Key << "Project" << YAML::Value << YAML::BeginMap;
			out << YAML::Key << "name" << YAML::Value << p.m_Name;
			out << YAML::Key << "author" << YAML::Value << p.m_Author;
			out << YAML::Key << "id" << YAML::Value << p.id;
			out << YAML::EndMap;
			out << YAML::EndMap;
			return out;
		}

		// Deserialization operator
		friend void operator>>(const YAML::Node& node, Project& p)
		{
			p.m_Name = node["name"].as<std::string>();
			p.m_Author = node["author"].as<std::string>();
			p.id = node["id"].as<uint64_t>();
		}


	};

}