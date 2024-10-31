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
		Mesh(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices);
		Mesh(Mesh& other)
		{
			va = other.va;
			vb = other.vb;
			ib = other.ib;
		}

		VertexArray GetVertexArray() { return va; }
		VertexBuffer GetVertexBuffer() { return vb; }
		IndexBuffer GetIndexBuffer() { return ib; }

		Mesh& operator =(const Mesh& other)
		{
			va = other.va;
			vb = other.vb;
			ib = other.ib;

			return *this;
		}

	private:
		void SetupMesh(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices);
		VertexArray va;
		VertexBuffer vb;
		IndexBuffer ib;
	};

}