#include "Application.h"
#include "AssetRegistry.h"
#include "Project.h"
#include <algorithm>
#include <fstream>

namespace Core
{

	Project Application::s_CurrentProject;

	Application::Application(const std::string& name, int window_width, int window_height)
	{

		m_Window = new Window(window_width, window_height, name);

		m_ImGuiLayer = new ImGuiLayer(this);
		m_ImGuiLayer->SetupImGui(this);

		Events::Dispatcher::Subscribe(std::bind(&ImGuiLayer::OnEvent, m_ImGuiLayer, std::placeholders::_1));
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

			// ImGui
			m_ImGuiLayer->BeginImGuiContent();

			for (auto& layer : m_Layers)
			{
				layer->OnImGuiRender();
			}

			m_ImGuiLayer->EndImGuiContent();
			Events::Dispatcher::ProcessEvents();
			m_Window->Update();
		}

		s_CurrentProject.Serialize(m_OpenedProjectPath);
	}

	void Application::OnEvent(Events::Event* event)
	{
	}

	Application::~Application()
	{
		delete m_ImGuiLayer;

		for (auto& layer : m_Layers)
		{
			layer->OnDettach();
			delete layer;
		}
		m_Layers.clear();

		m_Window->Close();
		s_CurrentProject.GetRegistry().Free();
	}

	void Application::LoadProject(const std::string& filename)
	{
		m_OpenedProjectPath = filename;
		s_CurrentProject.Deserialize(m_OpenedProjectPath);
		m_Window->SetTitle(s_CurrentProject.m_Name);
	}

}