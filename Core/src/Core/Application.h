#include <vector>
#include <string>
#include "Window.h"
#include "Events/Event.h"
#include "Layer.h"
#include "Project.h"

#pragma once

int main(int argc, char** argv);

namespace Core
{
	struct ApplicationCommandLineArgs
	{
		int Count = 0;
		char** Args = nullptr;

		const char* operator[](int index) const
		{
			ASSERT(index < Count);
			return Args[index];
		}
	};

	class Application
	{
	public:
		Application(const std::string& name = "No Project opened", int window_width = 1080, int window_height = 720);

		void PushLayer(Layer* layer);
		void PopLayer(Layer* layer);
		
		void Run();
		virtual void Update() = 0;

		void OnEvent(Events::Event*);

		~Application();

		inline Window* GetWindow() { return m_Window; }
		inline Project& GetCurrentProject() { return m_CurrentProject; }
		static Application* Get() { return s_Instance; }
		void LoadProject(const std::string& filename);

	protected:
		Window* m_Window;
		std::vector<Layer*> m_Layers;
		Project m_CurrentProject;
		std::string m_OpenedProjectPath;

		static Application* s_Instance;
	};

	// Defined in the client (Editor / Runtime)
	Application* CreateApplication(ApplicationCommandLineArgs args);

}