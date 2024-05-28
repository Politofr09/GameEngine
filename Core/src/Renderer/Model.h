#include <string>
#include <vector>
#include "Mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#pragma once

namespace Core::Gfx
{

	class Model
	{
	public:		
		Model()
		{
			m_Meshes = {};
			m_Directory = "";
		}
		
		Model(std::vector<Mesh> meshes, std::string directory)
			: m_Meshes(meshes), m_Directory(directory) {}
		
		std::vector<Mesh> GetMeshes() { return m_Meshes; }

		static Model LoadModel(const std::string& path);

	private:
		std::vector<Mesh> m_Meshes;
		std::vector<Texture> m_LoadedTextures; // We keep track of the loaded textures so we don't load 2 times the same texture
		std::string m_Directory;

		void ProcessNode(aiNode* node, const aiScene* scene);
		Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
		std::vector<Texture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type);

	};

}