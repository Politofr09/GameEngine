#pragma once

#include "Mesh.h"
#include "Material.h"
#include "Core/Asset.h"

#include <string>
#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace Core::Gfx
{

	class Model : public Asset
	{
	public:
		DECLARE_ASSET_TYPE("Model")

		static Model& Create(AssetRegistry& registry, const std::string& path, const std::string& name);

		bool Load() override;
		bool UnLoad() override;

		Model() = default;

		Model(std::vector<Mesh> meshes, std::string directory)
			: m_Meshes(meshes), m_Directory(directory) {}
		
		Model(const Model& other)
		{
			m_Directory = other.m_Directory;
			m_LoadedTextures = other.m_LoadedTextures;
			m_Material = other.m_Material;
			m_Meshes = other.m_Meshes;
		}

		Model& operator=(const Model& other)
		{
			m_Directory = other.m_Directory;
			m_LoadedTextures = other.m_LoadedTextures;
			m_Material = other.m_Material;
			m_Meshes = other.m_Meshes;

			return *this;
		}

		std::vector<Mesh> GetMeshes() { return m_Meshes; }
		Material& GetMaterial() { return m_Material; }

	private:
		Model(AssetRegistry* registry, const std::string& path, const std::string& name);

	private:
		std::vector<Mesh> m_Meshes;
		std::vector<Texture> m_LoadedTextures; // We keep track of the loaded textures so we don't load 2 times the same texture
		std::string m_Directory;
		Material m_Material;
		AssetRegistry* m_Registry; // Not ideal but it works

		void ProcessNode(aiNode* node, const aiScene* scene);
		Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
	};

}