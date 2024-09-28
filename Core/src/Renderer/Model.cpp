#include <GL/glew.h>
#include <fstream>
#include <sstream>
#include <vector>

#include "Model.h"
#include "Core/Utils.h"

namespace Core::Gfx
{

	Model& Model::Create(AssetRegistry& registry, const std::string& path, const std::string& name)
	{
		Model* model = new Model(&registry, path, name);

		if (model->Load())
		{
			registry.Track(model);
			return *model;
		}

		return Model();
	}

	Model::Model(AssetRegistry* registry, const std::string& path, const std::string& name)
	{
		m_Registry = registry;
		m_Directory = path;
		m_Name = name;
	}

	bool Model::Load()
	{
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(m_Directory, aiProcess_FlipUVs | aiProcess_Triangulate);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			LOG_ERROR("ERROR::ASSIMP::" + std::string(importer.GetErrorString()));
			return false;
		}

		m_Directory = m_Directory.substr(0, m_Directory.find_last_of('/'));

		// Process recursively root node
		ProcessNode(scene->mRootNode, scene);

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
			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

			m_Material = Material::CreateFromAssimp(*m_Registry, material, m_Directory);
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