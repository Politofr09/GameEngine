#include <vector>
#include <string>
#include "Window.h"
#include "Events/Event.h"
#include "Layer.h"
#include "ImGuiLayer.h"

#pragma once

namespace Core
{

	class Application
	{
	public:
		Application(const std::string& name = "Proton app", int window_width = 1080, int window_height = 720);
		const std::string& GetName() { return m_Name; }

		void PushLayer(Layer* layer);
		void PopLayer(Layer* layer);
		void Run();

		///*IMPORTANT: BEFORE CASTING EVENT WITH static_cast CHECK IF IT'S MATCHING THE EXPECTED TYPE
		/// Event.GetType() = "myExpectedEvent" AND THEN CAST.
		void OnEvent(Events::Event*);

		~Application();

		inline Window* GetWindow() { return m_Window; }

	private:

		std::string m_Name;
		Window* m_Window;
		std::vector<Layer*> m_Layers;
		ImGuiLayer* m_ImGuiLayer;
	};

}