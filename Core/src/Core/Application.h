#include <vector>
#include <string>
#include "Window.h"
#include "Events/Event.h"
#include "Layer.h"
#include "ImGuiLayer.h"
#include "Project.h"

#pragma once

namespace Core
{

#define OPENED_PROJECT Core::Application::s_CurrentProject

	class Application
	{
	public:
		static Project s_CurrentProject;

		Application(const std::string& name = "No Project opened", int window_width = 1080, int window_height = 720);

		void PushLayer(Layer* layer);
		void PopLayer(Layer* layer);
		void Run();

		///*IMPORTANT: BEFORE CASTING EVENT WITH static_cast CHECK IF IT'S MATCHING THE EXPECTED TYPE
		/// Event.GetType() = "myExpectedEvent" AND THEN CAST.
		void OnEvent(Events::Event*);

		~Application();

		inline Window* GetWindow() { return m_Window; }

		void LoadProject(const std::string& filename);
	private:

		Window* m_Window;
		std::vector<Layer*> m_Layers;
		ImGuiLayer* m_ImGuiLayer;
		std::string m_OpenedProjectPath;
	};

}