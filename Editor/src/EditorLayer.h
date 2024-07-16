#include <GL/glew.h>
#include "Core/Layer.h"
#include "Renderer/Shader.h"
#include "Renderer/Camera.h"
#include "Renderer/OrthographicCamera.h"
#include "Core/Utils.h"
#include "Events/InputEvents.h"
#include "Events/WindowEvents.h"
#include "Renderer/Texture.h"
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#pragma once

class EditorLayer : public Core::Layer
{
public:
    EditorLayer(Core::Window* window) : Layer(window) {};
	
    void OnAttach() override;
	void OnUpdate() override;
	void OnImGuiRender() override;
	void OnEvent(Core::Events::Event* event) override;
	void OnDettach() override;

private:
    Core::Gfx::Camera m_Cam;
    Core::Gfx::OrthographicCamera m_Cam2d;
    Core::Gfx::FrameBuffer m_FrameBuffer;

    //void UpdateCameraController();
    void ShowCameraControlImgui(bool* p_open);
    void DrawViewportImGui();
};