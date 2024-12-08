#include "EditorLayer.h"
#include "Core/ImGuiLayer.h"
#include "Core/Application.h"
#include "Core/EntryPoint.h"

using namespace Core;

class EditorApp : public Application
{
public:
	EditorApp()
		: Application("Editor App")
	{
		PushLayer(new EditorLayer());
	}
};

// Entry point
Core::Application* Core::CreateApplication(ApplicationCommandLineArgs args)
{
	return new EditorApp();
}