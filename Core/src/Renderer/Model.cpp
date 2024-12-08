#include <GL/glew.h>
#include <fstream>
#include <sstream>
#include <vector>

#include "Model.h"
#include "Core/Utils.h"
#include "Core/Application.h"
#include "Core/Instrumentor.h"

namespace Core::Gfx
{

	Ref<Model> Model::Create(const std::string& path)
	{
		AssetMetadata metadata;
		metadata.Path = path;
		metadata.ID = UUID();
		metadata.Name = "Model" + std::to_string(metadata.ID);
		metadata.Type = "Model";
		return Model::Create(metadata);
	}

	Ref<Model> Model::Create(const AssetMetadata& metadata)
	{
		Ref<Model> model = CreateRef<Model>(metadata);
		model->Load();
		return model;
	}

	bool Model::Load()
	{
		CORE_PROFILE_FUNCTION();

		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(m_Metadata.Path, aiProcess_FlipUVs | aiProcess_Triangulate | aiProcess_OptimizeMeshes | aiProcess_OptimizeGraph);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			LOG_ERROR("ERROR::ASSIMP::" + std::string(importer.GetErrorString()));
			return false;
		}

		// Process recursively root node
		ProcessNode(scene->mRootNode, scene);

		m_Material = Material::CreateFromMemory(m_AiMaterial, m_Metadata.Path);

		importer.FreeScene();

		return m_Loaded = true;
	}

	bool Model::UnLoad()
	{
		return false;
	}

	void Model::ProcessNode(aiNode* node, const aiScene* scene)
	{
		CORE_PROFILE_FUNCTION();
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			m_AiMaterial = scene->mMaterials[mesh->mMaterialIndex];

			m_Meshes.push_back(ProcessMesh(mesh, scene));
		}
		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			ProcessNode(node->mChildren[i], scene);
		}
	}

	Mesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
	{
		CORE_PROFILE_FUNCTION();
		Vertices vertices;
		std::vector<unsigned int> indices;

		vertices.Positions.reserve(mesh->mNumVertices);
		vertices.Normals.reserve(mesh->mNumVertices);
		vertices.TexCoords.reserve(mesh->mNumVertices);
		
		CORE_PROFILE_SCOPE("For loop 1"); {
			for (unsigned int i = 0; i < mesh->mNumVertices; i++)
			{
				vertices.Positions.emplace_back(glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z));

				vertices.Normals.emplace_back(mesh->HasNormals() ? glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z) : glm::vec3(0.0f, 0.0f, 0.0f));

				vertices.TexCoords.emplace_back(mesh->mTextureCoords[0] ? glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y) : glm::vec2(0.0f, 0.0f));
			}
		}

		CORE_PROFILE_SCOPE("For loop 2"); {
			indices.reserve(mesh->mNumFaces * 3);
			for (unsigned int i = 0; i < mesh->mNumFaces; ++i)
			{
				aiFace& face = mesh->mFaces[i];
				indices.insert(indices.end(), face.mIndices, face.mIndices + face.mNumIndices);
			}
		}

		return Mesh(vertices, indices);
	}

}