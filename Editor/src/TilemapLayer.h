#include "Renderer/OrthographicCamera.h"
#include "Renderer/FrameBuffer.h"
#include "Renderer/Texture.h"
#include "Renderer/Renderer2D.h"
#include "Renderer/Renderer.h"
#include "Renderer/TextRenderer.h"

#include "Core/Layer.h"

#pragma once

class TilemapLayer : public Core::Layer
{
public:
    TilemapLayer(Core::Window* window) : Layer(window) {};
	
    void OnAttach() override;
	void OnUpdate() override;
	void OnImGuiRender() override;
	void OnEvent(Core::Events::Event* event) override;
	void OnDettach() override;

private:
    void DrawViewport();

private:
    Core::Gfx::OrthographicCamera m_Camera;
    Core::Gfx::FrameBuffer m_FrameBuffer;
    Core::Gfx::Texture m_Tileset;
};