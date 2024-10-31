#include "EditorLayer.h"
#include "ImGuiLayer.h"
#include "Core/Application.h"
#include "Core/EntryPoint.h"

using namespace Core;

class EditorApp : public Application
{
public:
	EditorApp()
		: Application("Editor App")
	{
		m_ImGuiLayer = new ImGuiLayer();
		m_ImGuiLayer->SetupImGui();

		PushLayer(new EditorLayer());
	}

	void Update()
	{
		m_ImGuiLayer->BeginImGuiContent();

		for (auto layer : m_Layers)
		{
			layer->OnImGuiRender();
		}

		m_ImGuiLayer->EndImGuiContent();
	}

private:
	ImGuiLayer* m_ImGuiLayer = nullptr;
};

// Entry point
Core::Application* Core::CreateApplication(ApplicationCommandLineArgs args)
{
	return new EditorApp();
}