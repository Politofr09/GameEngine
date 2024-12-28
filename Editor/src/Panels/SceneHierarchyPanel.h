#pragma once

#include "Ecs/Entity.h"
#include "Core/Scene.h"

class SceneHierarchyPanel
{
public:
	SceneHierarchyPanel() = default;
	SceneHierarchyPanel(Core::Scene& scene);

	void SetContext(Core::Scene& scene);

	void OnImGuiRender();

	void SelectEntity(Core::Ecs::Entity entity) { m_SelectedEntity = entity; };
	Core::Ecs::Entity& GetSelectedEntity() { return m_SelectedEntity; }
	
private:
	void DrawEntityNode(Core::Ecs::Entity entity);

private:
	Core::Scene& m_ContextScene;
	Core::Ecs::Entity m_SelectedEntity;
};