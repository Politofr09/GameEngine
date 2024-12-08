#include "Mesh.h"

using namespace Core::Gfx;

Mesh::Mesh(Vertices& vertices, std::vector<unsigned int>& indices)
{
	SetupMesh(vertices, indices);
}

void Mesh::SetupMesh(Vertices& vertices, std::vector<unsigned int>& indices)
{
    // Initialize and bind the Vertex Array
    va.Init();
    va.Bind();

    // Position attribute
    BufferLayout positionLayout;
    positionLayout.Push<float>(3);
    VertexBuffer positionBuffer(vertices.Positions.data(), vertices.Positions.size() * sizeof(glm::vec3));
    va.AddBuffer(positionBuffer, positionLayout);

    //// Normal attribute
    BufferLayout normalLayout;
    normalLayout.Push<float>(3);
    VertexBuffer normalBuffer(vertices.Normals.data(), vertices.Normals.size() * sizeof(glm::vec3));
    va.AddBuffer(normalBuffer, normalLayout);

    // Texcoords attribute
    BufferLayout texcoordLayout;
    texcoordLayout.Push<float>(2);
    VertexBuffer texcoordsBuffer(vertices.TexCoords.data(), vertices.TexCoords.size() * sizeof(glm::vec2));
    va.AddBuffer(texcoordsBuffer, texcoordLayout);

    IndexBuffer indexBuffer(indices.data(), indices.size());
    va.SetIndexBuffer(indexBuffer);
}