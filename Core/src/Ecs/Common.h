#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/euler_angles.hpp>

#include "Core/Asset.h"

namespace Core::Ecs
{

	struct NameComponent
	{
		std::string Name;
	};

	struct PositionComponent
	{
		float x;
		float y;
		float z;

		operator glm::vec3() const { return glm::vec3(x, y, z); }
		PositionComponent& operator=(const glm::vec3& vec)
		{
			x = vec.x;
			y = vec.y;
			z = vec.z;
			return *this;
		}
	};

	struct ScaleComponent
	{
		float x;
		float y;
		float z;

		operator glm::vec3() const { return glm::vec3(x, y, z); }
		ScaleComponent& operator=(const glm::vec3& vec)
		{
			x = vec.x;
			y = vec.y;
			z = vec.z;
			return *this;
		}
	};

	struct RotationComponent
	{
		float x;
		float y;
		float z;

		operator glm::vec3() const { return glm::vec3(x, y, z); }
		RotationComponent& operator=(const glm::vec3& vec)
		{
			x = vec.x;
			y = vec.y;
			z = vec.z;
			return *this;
		}
	};

	struct TransformComponent
	{
		PositionComponent Position;
		RotationComponent Rotation;
		ScaleComponent Scale = { 1.0f, 1.0f, 1.0f };

		operator glm::mat4() const
		{
			return glm::translate(glm::mat4(1.0f), glm::vec3(Position.x, Position.y, Position.z)) *
				glm::yawPitchRoll(glm::radians(Rotation.y), glm::radians(Rotation.x), glm::radians(Rotation.z)) *
				glm::scale(glm::mat4(1.0f), glm::vec3(Scale.x, Scale.y, Scale.z));
		}
	};

	struct ModelComponent
	{
		UUID ModelHandle = 0;
	};

	struct SpriteComponent
	{
		UUID TextureHandle = 0;
		glm::vec4	Color;
	};

	// TODO: Add more of them! Fill this file with components!!!
}