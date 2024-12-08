#include "Texture.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include <glm/glm.hpp>
#include <vector>

#pragma once

namespace Core::Gfx
{

	struct Vertices
	{
		std::vector<glm::vec3> Positions;
		std::vector<glm::vec3> Normals;
		std::vector<glm::vec2> TexCoords;
	};

	class Mesh
	{
	public:
		Mesh(Vertices& vertices, std::vector<unsigned int>& indices);
		Mesh(Mesh& other)
		{
			va = other.va;
		}

		VertexArray GetVertexArray() const { return va; }

		Mesh& operator =(const Mesh& other)
		{
			va = other.va;

			return *this;
		}

	private:
		void SetupMesh(Vertices& vertices, std::vector<unsigned int>& indices);
		VertexArray va;
	};

}