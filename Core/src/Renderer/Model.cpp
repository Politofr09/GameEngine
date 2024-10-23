#include <GL/glew.h>
#include <fstream>
#include <sstream>
#include <vector>

#include "Model.h"
#include "Core/Utils.h"
#include "Core/Application.h"

namespace Core::Gfx
{

	AssetHandle Model::Create(const std::string& path, const std::string& name)
	{
		std::cout << "Creating model" << std::endl;

		AssetMetadata metadata
		{
			name,
			path,
			UUID()
		};
		// Provide dummy asset handle
		Model* model = new Model(metadata, 0);

		if (model->Load())
		{
			// Check if we can retrieve material from the registry to avoid duplication
			// Materials actually point to a full 3d model / scene file
			// Thus we can check if there is a material with the same path

			if (AssetHandle handle = OPENED_PROJECT.GetRegistry().FindByPath<Material>(model->GetPath()) != 0)
			{
				model->SetMaterialHandle(handle);
			}
			else
			{
				model->SetMaterialHandle(Material::Create(model->GetPath(), model->GetName() + "_material"));
			}

			return OPENED_PROJECT.GetRegistry().Track(model);
		}

		return 0;
	}

	Model::Model(const AssetMetadata& metadata, AssetHandle material)
		: Asset(metadata), m_MaterialHandle(material) {}

	bool Model::Load()
	{
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(m_Metadata.Path, aiProcess_FlipUVs | aiProcess_Triangulate);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			LOG_ERROR("ERROR::ASSIMP::" + std::string(importer.GetErrorString()));
			return false;
		}

		m_Directory = m_Metadata.Path.substr(0, m_Metadata.Path.find_last_of('/'));

		// Process recursively root node
		ProcessNode(scene->mRootNode, scene);

		m_Loaded = true;

		return true;
	}

	bool Model::UnLoad()
	{
		return false;
	}

	void Model::ProcessNode(aiNode* node, const aiScene* scene)
	{
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			m_AiMaterial = scene->mMaterials[mesh->mMaterialIndex];

			//m_Material = Material::CreateFromAssimp(*m_Registry, material, m_Directory);
			m_Meshes.push_back(ProcessMesh(mesh, scene));
		}
		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			ProcessNode(node->mChildren[i], scene);
		}
	}

	Mesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
	{
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;

		vertices.reserve(mesh->mNumVertices);
		indices.reserve(mesh->mNumFaces * 3);

		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			Vertex vertex{};

			glm::vec3 pos{};
			pos.x = mesh->mVertices[i].x;
			pos.y = mesh->mVertices[i].y;
			pos.z = mesh->mVertices[i].z;
			vertex.Position = pos;

			glm::vec3 normal{ 0, 0, 0 };
			if (mesh->HasNormals())
			{
				normal.x = mesh->mNormals[i].x;
				normal.y = mesh->mNormals[i].y;
				normal.z = mesh->mNormals[i].z;
			}
			vertex.Normal = normal;

			glm::vec2 texCoord{ 0, 0 };
			if (mesh->mTextureCoords[0])
			{
				texCoord.x = mesh->mTextureCoords[0][i].x;
				texCoord.y = mesh->mTextureCoords[0][i].y;
			}
			vertex.TexCoords = texCoord;

			vertices.push_back(vertex);
		}

		// Process the indices
		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			for (unsigned int j = 0; j < face.mNumIndices; j++)
				indices.push_back(face.mIndices[j]);
		}

		return Mesh(vertices, indices);
	}

}