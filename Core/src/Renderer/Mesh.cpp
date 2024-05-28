#include "Mesh.h"

using namespace Core::Gfx;

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures)
{
    this->textures = textures;
	SetupMesh(vertices, indices, textures);
}

void Mesh::SetupMesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures)
{
    va.Init();
    va.Bind();

    vb = VertexBuffer(vertices.data(), vertices.size() * sizeof(Vertex));
    vb.Bind();

    ib = IndexBuffer(indices.data(), indices.size());
    ib.Bind();

    BufferLayout layout;
    layout.Push<float>(3); // Position attribute
    layout.Push<float>(3); // Normal attribute
    layout.Push<float>(2); // uv attribute
    va.AddBuffer(vb, layout);
}
