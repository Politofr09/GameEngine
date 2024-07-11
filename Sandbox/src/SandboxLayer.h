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

class SandboxLayer : public Core::Layer
{
public:
    SandboxLayer(Core::Window* window) : Layer(window) {};
	
    void OnAttach() override;
	void OnUpdate() override;
	void OnImGuiRender() override;
	void OnEvent(Core::Events::Event* event) override;
	void OnDettach() override;

private:
    Core::Gfx::Camera cam;
    Core::Gfx::OrthographicCamera cam2d;

    void UpdateCameraController();
    void ShowCameraControlImgui(bool* p_open);
};