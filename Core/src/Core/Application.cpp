#include "Application.h"
#include "AssetRegistry.h"
#include "Project.h"
#include <algorithm>
#include <fstream>

namespace Core
{

	Application* Application::s_Instance = nullptr;

	Application::Application(const std::string& name, int window_width, int window_height)
	{
		s_Instance = this;
		m_Window = new Window(window_width, window_height, name);

		Events::Dispatcher::Subscribe(std::bind(&Application::OnEvent, this, std::placeholders::_1));

		LoadProject("Test1.yaml");
	}

	void Application::PushLayer(Layer* layer)
	{
		layer->OnAttach();
		m_Layers.push_back(layer);
	}

	void Application::PopLayer(Layer* layer)
	{
		auto it = std::find(m_Layers.begin(), m_Layers.end(), layer);
		if (it != m_Layers.end())
		{
			m_Layers.erase(it);
			layer->OnDettach();
			delete(layer);
		}
	}

	void Application::Run()
	{
		while (!m_Window->ShouldClose())
		{
			for (auto& layer : m_Layers)
			{
				layer->OnUpdate();
			}

			//// ImGui
			//m_ImGuiLayer->BeginImGuiContent();

			//for (auto& layer : m_Layers)
			//{
			//	layer->OnImGuiRender();
			//}

			//m_ImGuiLayer->EndImGuiContent();

			// Client specific logic
			Update();

			Events::Dispatcher::ProcessEvents();
			m_Window->Update();
		}

	}

	void Application::OnEvent(Events::Event* event)
	{
	}

	Application::~Application()
	{
		m_CurrentProject.Serialize(m_OpenedProjectPath);

		for (auto& layer : m_Layers)
		{
			layer->OnDettach();
			delete layer;
		}
		m_Layers.clear();

		m_Window->Close();
		m_CurrentProject.GetRegistry().Free();
	}

	void Application::LoadProject(const std::string& filename)
	{
		m_OpenedProjectPath = filename;
		m_CurrentProject.Deserialize(m_OpenedProjectPath);
		m_Window->SetTitle(m_CurrentProject.m_Name);
	}

}