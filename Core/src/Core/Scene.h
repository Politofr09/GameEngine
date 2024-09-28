#pragma once

#include "Ecs/Entity.h"
#include "Ecs/ECS.h"

namespace Core
{

	class Scene
	{
	public:
		Scene();

		Ecs::Entity CreateEntity(const std::string& name = "Unnamed entity");

	private:
		Ecs::ECS& m_Registry;
	};

}