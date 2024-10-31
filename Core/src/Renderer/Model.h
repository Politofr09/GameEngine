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

		static AssetHandle Create(const std::string& path, const std::string& name);

		bool Load() override;
		bool UnLoad() override;

		Model() = default;

		Model(std::vector<Mesh> meshes, std::string directory)
			: m_Meshes(meshes), m_Directory(directory) {}
		
		
		Model(const AssetMetadata& metadata, AssetHandle material);
		
		Model(const Model& other)
		{
			m_Directory = other.m_Directory;
			m_MaterialHandle = other.m_MaterialHandle;
			m_Meshes = other.m_Meshes;
		}

		Model& operator=(const Model& other)
		{
			m_Directory = other.m_Directory;
			m_MaterialHandle = other.m_MaterialHandle;
			m_Meshes = other.m_Meshes;

			return *this;
		}

		std::vector<Mesh> GetMeshes() { return m_Meshes; }
		AssetHandle GetMaterialHandle() const { return m_MaterialHandle; }
		void SetMaterialHandle(AssetHandle handle) { m_MaterialHandle = handle; }

		aiMaterial* GetAssimpMaterial() { return m_AiMaterial; }
	private:

	private:
		std::vector<Mesh> m_Meshes;
		std::string m_Directory; // Relative path for material / texture references

		AssetHandle m_MaterialHandle;
		aiMaterial* m_AiMaterial = nullptr;
		

		void ProcessNode(aiNode* node, const aiScene* scene);
		Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
	};

}