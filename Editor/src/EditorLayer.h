#pragma once

#include <GL/glew.h>
#include "Core/Layer.h"
#include "Renderer/Shader.h"
#include "Renderer/Camera.h"
#include "Renderer/OrthographicCamera.h"
#include "Core/Utils.h"
#include "Panels/SceneHierarchyPanel.h"
#include "Panels/ContentBrowserPanel.h"
#include "Events/InputEvents.h"
#include "Events/WindowEvents.h"
#include "Renderer/Texture.h"
#include "Renderer/FrameBuffer.h"
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class EditorLayer : public Core::Layer
{
public:
    EditorLayer();
	
    void OnAttach() override;
	void OnUpdate() override;
	void OnImGuiRender() override;
	void OnEvent(Core::Events::Event* event) override;
	void OnDettach() override;

private:
    Core::Gfx::Camera m_Cam;
    Core::Gfx::OrthographicCamera m_Cam2d;
    Core::Gfx::FrameBuffer m_Framebuffer;

    int m_GizmoOperation = 0;

    //void UpdateCameraController();
    void DrawViewportImGui();

    // UI: (BIG TODO: EditorLayer is like 700 lines...)
    bool showCameraControl = true;
    void ShowCameraControlImgui(bool* p_open);

    bool showThemeSwitcher = true;
    void ShowThemeSwitcher(bool* p_open);

    bool showAssetRegistry = true;
    void ShowAssetRegistry(bool* p_open);

    bool showRenderingSettings = true;
    void ShowRenderingSettings(bool* p_open);

    SceneHierarchyPanel m_SceneHierarchyPanel;
    ContentBrowserPanel m_ContentBrowserPanel;
    
};