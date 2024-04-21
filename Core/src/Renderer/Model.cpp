#include <GL/glew.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <iostream> // For debugging
#include <glm/glm.hpp>

#include "Model.h"
#include "Core/Utils.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

Core::Gfx::Model Core::Gfx::LoadFromOBJ(const std::string& path, VertexArray& va)
{
    std::ifstream file(path);
    if (!file.is_open())
    {
        LOG_ERROR("Failed to open obj file: " + path);
        return Model();
    }

    // Vertex data
    std::vector<glm::vec3> positions;
    std::vector<glm::vec2> uvs;
    std::vector<glm::vec3> normals;

    // Indices
    std::vector<unsigned int> positionIndices, uvIndices, normalIndices;

    std::string currentLine;
    while (std::getline(file, currentLine))
    {
        std::istringstream iss(currentLine);
        std::string prefix; // "v"; "f"; "vn"; "vt"
        iss >> prefix;

        if (prefix == "v") // Vertex position 3D vector
        {
            glm::vec3 position{};
            iss >> position.x >> position.y >> position.z;
            positions.push_back(position);
        }
        else if (prefix == "vt") // Vertex uv 2D vector
        {
            glm::vec2 uv{};
            iss >> uv.x >> uv.y;
            uvs.push_back(uv);
        }
        else if (prefix == "vn") // Vertex normal 3D vector
        {
            glm::vec3 normal{};
            iss >> normal.x >> normal.y >> normal.z;
            normals.push_back(normal);
        }
        else if (prefix == "f")
        {
            for (unsigned int i = 0; i < 3; i++)
            {
                std::string faceVertex;
                iss >> faceVertex;

                unsigned int positionIndex, uvIndex, normalIndex;

                std::istringstream faceVertexStream(faceVertex);
                char slash;
                faceVertexStream >> positionIndex >> slash >> uvIndex >> slash >> normalIndex;

                positionIndices.push_back(positionIndex - 1);
                uvIndices.push_back(uvIndex - 1);
                normalIndices.push_back(normalIndex - 1);
            }
        }
    }

    // Interleave the data
    std::vector<float> interleavedData;
    for (unsigned int i = 0; i < positionIndices.size(); i++)
    {
        unsigned int positionIndex = positionIndices[i];
        glm::vec3& position = positions[positionIndex];
        interleavedData.push_back(position.x);
        interleavedData.push_back(position.y);
        interleavedData.push_back(position.z);

        unsigned int normalIndex = normalIndices[i];
        glm::vec3& normal = normals[normalIndex]; // Use the only available normal
        interleavedData.push_back(normal.x);
        interleavedData.push_back(normal.y);
        interleavedData.push_back(normal.z);

        unsigned int uvIndex = uvIndices[i];
        glm::vec2& uv = uvs[uvIndex];
        interleavedData.push_back(uv.x);
        interleavedData.push_back(uv.y);
    }

    // Now that we have the data loaded in the CPU and interleaved, we can load it in the GPU with OpenGL
    va.Bind();
    VertexBuffer vb(interleavedData.data(), sizeof(float) * interleavedData.size());
    LOG_INFO("Model loaded has " + std::to_string(positionIndices.size()) + " vertices");

    BufferLayout layout;
    layout.Push<float>(3); // Position attribute
    layout.Push<float>(3); // Normal attribute
    layout.Push<float>(2); // uv attribute
    va.AddBuffer(vb, layout);

    //std::reverse(positionIndices.begin(), positionIndices.end());

    //positionIndices[positionIndices.size() - 1]++;
    IndexBuffer ib(positionIndices.data(), positionIndices.size());

    return Model(va.GetRendererID(), ib, vb);
}
