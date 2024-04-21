#include <GL/glew.h>
#include "Core/Layer.h"
#include "Core/Shader.h"
#include "Core/Camera.h"
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
	void OnEvent(Core::Events::Event& event) override;
	void OnDettach() override;

private:
    
    glm::mat4 trans= glm::mat4();
    Core::Camera cam;

    Shader shader;
    Core::Gfx::Texture texture;

    void UpdateCameraController();
    void ShowCameraControlImgui(bool* p_open);
};