#include "Scene.h"

namespace Core
{

	using namespace Core::Ecs;

	Entity Scene::CreateEntity(const std::string& name)
	{
		return Entity(m_Registry.CreateEntity(), m_Registry, name);
	}

}