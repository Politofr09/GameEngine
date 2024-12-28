#include "SceneHierarchyPanel.h"
#include <imgui/imgui.h>
#include <imgui/IconsFontAwesome5.h>
#include "Renderer/Material.h"
#include "Renderer/Model.h"
#include "Renderer/Shader.h"
#include "Core/Application.h"

using namespace Core;
using namespace Ecs;

SceneHierarchyPanel::SceneHierarchyPanel(Scene& scene)
	: m_SelectedEntity(Ecs::Entity(0, scene.GetRegistry())), m_ContextScene(scene)
{
}

void SceneHierarchyPanel::SetContext(Scene& scene)
{
	m_ContextScene = scene;
}

static bool entityDeleted = false;

void SceneHierarchyPanel::DrawEntityNode(Entity entity)
{
	auto& name = entity.GetName();

	ImGuiTreeNodeFlags flags = ((m_SelectedEntity == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
	flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
	bool opened = ImGui::TreeNodeEx((void*)(uint64_t)entity.GetID(), flags, name.c_str());
	if (ImGui::IsItemClicked())
	{
		m_SelectedEntity = entity;
	}

	entityDeleted = false;
	if (ImGui::BeginPopupContextItem())
	{
		if (ImGui::MenuItem("Delete Entity"))
			entityDeleted = true;

		ImGui::EndPopup();
	}

	if (opened)
	{
		ImGui::TreePop();
	}

	if (entityDeleted) m_SelectedEntity.Kill();
}

template <typename T, typename UIFunction>
static void DrawComponent(const std::string& name, Entity entity, UIFunction uiFunction)
{
	if (entity.HasComponent<T>())
	{
		static bool open = true;

		// Start the child window with a border to create the framed effect
		ImGuiWindowFlags childWindowFlags = ImGuiWindowFlags_None;
		if (!open) childWindowFlags |= ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;
		ImGui::BeginChild(name.c_str(), ImVec2(0, 0), ImGuiChildFlags_FrameStyle | ImGuiChildFlags_AutoResizeY, childWindowFlags);

		ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen
			| ImGuiTreeNodeFlags_Framed
			| ImGuiTreeNodeFlags_SpanAvailWidth
			| ImGuiTreeNodeFlags_AllowItemOverlap
			| ImGuiTreeNodeFlags_FramePadding;


		open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodeFlags, name.c_str());
		if (open)
		{
			uiFunction(entity.GetComponent<T>());
			
			if (ImGui::Button("Remove"))
			{
				entity.RemoveComponent<T>();
			}
			
			ImGui::TreePop();
		}

		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Remove"))
				entity.RemoveComponent<T>();

			ImGui::EndPopup();
		}

		ImGui::EndChild();
	}
}

static void DrawComponents(Entity entity)
{
	if (entity.HasComponent<NameComponent>())
	{
		auto& name = entity.GetComponent<NameComponent>().Name;

		char buffer[256];
		memset(buffer, 0, sizeof(buffer));
#ifdef _WIN32
		strncpy_s(buffer, sizeof(buffer), name.c_str(), sizeof(buffer));
#else 
		std::strncpy(buffer, name.c_str(), sizeof(buffer) - 1); // Ensure null termination
		buffer[sizeof(buffer) - 1] = '\0';                      // Explicitly null terminate
#endif
		
		ImGui::SetCursorPosX(10.0f);
		if (ImGui::InputText("##Name", buffer, sizeof(buffer)))
		{
			name = std::string(buffer);
		}
	}

	ImGui::SameLine();
	if (ImGui::Button("Add Component"))
	{
		ImVec2 button_pos = ImGui::GetItemRectMin();  // Get top-left corner of button
		ImVec2 button_size = ImGui::GetItemRectSize(); // Get button size
		ImGui::SetNextWindowPos(ImVec2(button_pos.x, button_pos.y + button_size.y)); // Set position directly below the button
		ImGui::OpenPopup("AddComponentPopup");
	}

	if (ImGui::BeginPopup("AddComponentPopup"))
	{
		if (ImGui::MenuItem("Transform"))
		{
			if (!entity.HasComponent<TransformComponent>())
			{
				entity.AddComponent<TransformComponent>(TransformComponent{});
			}
		}

		if (ImGui::MenuItem("Model"))
		{
			if (!entity.HasComponent<ModelComponent>())
			{
				entity.AddComponent<ModelComponent>(ModelComponent{ 0 });
			}
		}

		if (ImGui::MenuItem("Sprite"))
		{
			if (!entity.HasComponent<SpriteComponent>())
			{
				entity.AddComponent<SpriteComponent>(SpriteComponent{ 0, glm::vec4(1.0f) });
			}
		}
		ImGui::EndPopup();
	}

	DrawComponent<TransformComponent>("Transform", entity, [](auto& component)
	{
		ImGui::DragFloat3("Position", &component.Position.x, 0.1f);
		ImGui::DragFloat3("Rotation", &component.Rotation.x);
		ImGui::DragFloat3("Scale", &component.Scale.x, 0.1f);
	});

	DrawComponent<ModelComponent>("Model", entity, [](auto& component)
	{
		ImGui::Text("Model: %llu", component.ModelHandle);

		// Accept payload from drag and drop imgui (only model ids)
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ASSET_DND_Model"))
			{
				Core::UUID handle = *(uint64_t*)payload->Data;
				component.ModelHandle = handle;
			}
			ImGui::EndDragDropTarget();
		}

		if (!component.ModelHandle) return;
		Ref<Core::Gfx::Model> model = Core::Application::Get()->GetCurrentProject().GetRegistry().Get<Core::Gfx::Model>(component.ModelHandle);
		Ref<Core::Gfx::Material> material = model->GetMaterial();

		if (!model) return;
		if (!material) return;

		ImGui::Text("Material: %llu", model->GetMaterial()->ID);

		// Accept payload from drag and drop imgui (only model ids)
		if (ImGui::BeginDragDropTarget() && component.ModelHandle != 0)
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ASSET_DND_Material"))
			{
				Core::UUID handle = *(uint64_t*)payload->Data;
				model->SetMaterial(Core::Application::Get()->GetCurrentProject().GetRegistry().Get<Core::Gfx::Material>(handle));
			}
			ImGui::EndDragDropTarget();
		}

		if (component.ModelHandle != 0 && ImGui::TreeNode("Material editor"))
		{
			ImGui::ColorEdit3("Color", &material->Color[0]);
			ImGui::ColorEdit3("Ambient", &material->Ambient[0]);
			ImGui::ColorEdit3("Diffuse", &material->Diffuse[0]);
			ImGui::ColorEdit3("Specular", &material->Specular[0]);
			ImGui::SliderFloat("Shininess", &material->Shininess, 0.01f, 128.0f);

			ImGui::Text("DiffuseTexture: %llu", material->DiffuseTexture->GetMetadata().ID);
			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ASSET_DND_Texture"))
				{
					Core::UUID handle = *(uint64_t*)payload->Data;
					material->DiffuseTexture = Core::Application::Get()->GetCurrentProject().GetRegistry().Get<Core::Gfx::Texture>(handle);
				}
				ImGui::EndDragDropTarget();
			}

			ImGui::TreePop();
		}
	});

	DrawComponent<SpriteComponent>("Sprite", entity, [](auto& component)
	{
		ImGui::Text("Texture: %llu", component.TextureHandle);

		// Accept payload from drag and drop imgui (only model ids)
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ASSET_DND_Texture"))
			{
				Core::UUID handle = *(uint64_t*)payload->Data;
				component.TextureHandle = handle;
			}
			ImGui::EndDragDropTarget();
		}

		ImGui::SameLine();
		if (ImGui::Button(ICON_FA_TIMES))
			component.TextureHandle = 0;

		// Use sliders for position, rotation, and scale
		ImGui::ColorEdit4("Color", &component.Color.x); 
	});

}

void SceneHierarchyPanel::OnImGuiRender()
{
	ImGui::Begin("Scene Hierarchy");

	if (ImGui::Button("Add Entity"))
	{
		m_SelectedEntity = m_ContextScene.CreateEntity("New Entity");
		m_SelectedEntity.AddComponent<TransformComponent>(TransformComponent{});
	}

	auto& registry = m_ContextScene.GetRegistry();
	registry.each([&](ECS::EntityID entityID)
	{
		Entity entity{ entityID, registry };
		DrawEntityNode(entity);

		// if (entityDeleted)
		// {
		// 	m_SelectedEntity.Kill();
		// 	entityDeleted = false;
		// 	return;
		// }
	});

	ImGui::End();

	ImGui::Begin("Properties", 0, ImGuiWindowFlags_NoScrollbar);
	if (m_SelectedEntity.GetID())
	{
		DrawComponents(m_SelectedEntity);
	}
	ImGui::End();
}