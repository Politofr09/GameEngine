#include <glm/glm.hpp>

#pragma once

namespace Core::Gfx
{

	class Light
	{
	public:
        Light() = default;

        glm::vec3 Position  = { 0.0f, 0.0f, 0.0f };
        glm::vec3 Direction = { 0.0f, -1.0f, 0.0f };
        glm::vec3 Color     = { 1.0f, 1.0f, 1.0f };     // Overall light color
        glm::vec3 Ambient   = { 0.2f, 0.2f, 0.2f };     // Low light when not directly lit
        glm::vec3 Diffuse   = { 0.5f, 0.5f, 0.5f };     // Affected by direct light
        glm::vec3 Specular  = { 1.0f, 1.0f, 1.0f };     // Shiny reflection
	};

}