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

	class Model
	{
	public:
		static Ref<Model> Create(const std::string& path);
		static Ref<Model> Create(const AssetMetadata& metadata);

		bool Load();
		bool UnLoad();

		Model() = default;
		Model(AssetMetadata metadata)
			: m_Metadata(metadata) {}

		Model(AssetMetadata metadata, std::vector<Mesh> meshes)
			: m_Metadata(metadata), m_Meshes(meshes) {}
		
		Model(const Model& other)
		{
			m_Metadata = other.m_Metadata;
			m_Material = other.m_Material;
			m_Meshes = other.m_Meshes;
			m_Loaded = other.m_Loaded;
		}

		Model& operator=(const Model& other)
		{
			m_Metadata = other.m_Metadata;
			m_Material = other.m_Material;
			m_Meshes = other.m_Meshes;
			m_Loaded = other.m_Loaded;
			return *this;
		}

		std::vector<Mesh> GetMeshes() { return m_Meshes; }
		Ref<Material> GetMaterial() { return m_Material; }

		AssetMetadata& GetMetadata() { return m_Metadata; }
		bool IsLoaded() const { return m_Loaded; }

		void SetMaterial(Ref<Material> material) 
		{
			m_Metadata.Dependencies["Material"] = material->ID;
			m_Material = material; 
		}

		aiMaterial* GetAssimpMaterial() { return m_AiMaterial; }

	private:
		std::vector<Mesh> m_Meshes;
		AssetMetadata m_Metadata;
		bool m_Loaded = false;

		Ref<Material> m_Material;
		aiMaterial* m_AiMaterial = nullptr;
		
	private:
		void ProcessNode(aiNode* node, const aiScene* scene);
		Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
	};

}