#include "Mesh.h"

using namespace Core::Gfx;

Mesh::Mesh(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices)
{
	SetupMesh(vertices, indices);
}

void Mesh::SetupMesh(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices)
{
    va.Init();
    va.Bind();

    vb = VertexBuffer(vertices.data(), vertices.size() * sizeof(Vertex));

    ib = IndexBuffer(indices.data(), indices.size());

    BufferLayout layout;
    layout.Push<float>(3); // Position attribute
    layout.Push<float>(3); // Normal attribute
    layout.Push<float>(2); // uv attribute
    va.AddBuffer(vb, layout);
    va.SetIndexBuffer(ib);
}
