#include <GL/glew.h>
#include <fstream>
#include <sstream>
#include <vector>

#include "Model.h"
#include "Core/Utils.h"

using namespace Core::Gfx;

Model Model::LoadModel(const std::string& path)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_FlipUVs | aiProcess_Triangulate);
	
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		LOG_ERROR("ERROR::ASSIMP::" + std::string(importer.GetErrorString()));
		return Model(); // Empty model
	}

	std::string directory = path.substr(0, path.find_last_of('/'));
	Model model({}, directory);

	// Process recursively root node
	model.ProcessNode(scene->mRootNode, scene);

	return model;
}

void Model::ProcessNode(aiNode* node, const aiScene* scene)
{
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
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

	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

	//std::vector<Texture> diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE);
	//textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

	//std::vector<Texture> specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR);
	//textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

	//std::vector<Texture> ambientMaps = LoadMaterialTextures(material, aiTextureType_AMBIENT);
	//textures.insert(textures.end(), ambientMaps.begin(), ambientMaps.end());

	//std::vector<Texture> emissionMaps = LoadMaterialTextures(material, aiTextureType_EMISSIVE);
	//textures.insert(textures.end(), emissionMaps.begin(), emissionMaps.end());

	//std::vector<Texture> normalMaps = LoadMaterialTextures(material, aiTextureType_NORMALS);
	//textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

	//std::vector<Texture> heightMaps = LoadMaterialTextures(material, aiTextureType_HEIGHT);
	//textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

	LoadMaterial(material);
	return Mesh(vertices, indices);
}

// Not used :)
std::vector<Texture> Model::LoadMaterialTextures(aiMaterial* mat, aiTextureType type)
{
	std::vector<Texture> textures;

	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		bool skip = false;

		for (unsigned int j = 0; j < m_LoadedTextures.size(); j++)
		{
			if (std::strcmp(m_LoadedTextures[j].GetFileDirectory().data(), str.C_Str()) == 0)
			{
				textures.push_back(m_LoadedTextures[j]);
				skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
				break;
			}
		}
		if (!skip) // If texture doesn't exist already
		{
			std::string tex_type = "texture_diffuse";

			switch (type)
			{
			case aiTextureType_DIFFUSE:     tex_type = "texture_diffuse";		break;
			case aiTextureType_SPECULAR:    tex_type = "texture_specular";		break;
			case aiTextureType_AMBIENT:     tex_type = "texture_ambient";		break;
			case aiTextureType_EMISSIVE:    tex_type = "texture_emissive";		break;
			case aiTextureType_NORMALS:     tex_type = "texture_normal";		break;
			case aiTextureType_HEIGHT:      tex_type = "texture_height";		break;
			default:                        tex_type = "texture_diffuse";       break;
			}

			Texture texture = Texture::Create(m_Directory + "/" + str.C_Str(), tex_type);

			// LOG_INFO("Loaded texture: " + texture.GetPath() + " with type: " + aiTextureTypeToString(type));

			textures.push_back(texture);
			m_LoadedTextures.push_back(texture);
		}
	}
	return textures;
}

void Model::LoadMaterial(aiMaterial* mat)
{
	aiString str;
	mat->GetTexture(aiTextureType_DIFFUSE, 0, &str);
	Texture texture = Texture::Create(m_Directory + "/" + str.C_Str(), "model_texture" + std::string(mat->GetName().C_Str()), "texture_diffuse");
	m_Material.SetTexture(texture);
	m_Material.SetMatrix("uTransform", glm::mat4(1.0f));
}