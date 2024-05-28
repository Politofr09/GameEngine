#include "Texture.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include <glm/glm.hpp>
#include <vector>

#pragma once

namespace Core::Gfx
{

	struct Vertex
	{
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 TexCoords;
	};

	class Mesh
	{
	public:
		std::vector<Texture> textures;

		Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);

		VertexArray GetVertexArray() { return va; }
		VertexBuffer GetVertexBuffer() { return vb; }
		IndexBuffer GetIndexBuffer() { return ib; }

	private:
		void SetupMesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
		VertexArray va;
		VertexBuffer vb;
		IndexBuffer ib;
	};

}