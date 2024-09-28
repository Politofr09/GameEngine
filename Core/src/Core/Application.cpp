#include "Application.h"
#include "AssetRegistry.h"
#include "Project.h"
#include <algorithm>
#include <fstream>

#include <thread>

Core::Application::Application(const std::string& name, int window_width, int window_height)
{
	m_Window = new Window(window_width, window_height, name);

	m_ImGuiLayer = new ImGuiLayer(this);
	m_ImGuiLayer->SetupImGui(this);

	Events::Dispatcher::Subscribe(std::bind(&ImGuiLayer::OnEvent, m_ImGuiLayer, std::placeholders::_1));
	Events::Dispatcher::Subscribe(std::bind(&Application::OnEvent, this, std::placeholders::_1));

	LoadProject("Test1.yaml");
}

void Core::Application::PushLayer(Layer* layer)
{
	layer->OnAttach();
	m_Layers.push_back(layer);
}

void Core::Application::PopLayer(Layer* layer)
{
	auto it = std::find(m_Layers.begin(), m_Layers.end(), layer);
	if (it != m_Layers.end())
	{
		m_Layers.erase(it);
		layer->OnDettach();
		delete(layer);
	}
}

void Core::Application::Run()
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
}

void Core::Application::OnEvent(Core::Events::Event* event)
{
}

Core::Application::~Application()
{
	delete m_ImGuiLayer;

	for (auto& layer : m_Layers)
	{
		layer->OnDettach();
		delete layer;
	}
	m_Layers.clear();

	m_Window->Close();
	m_Project.registry.Free();
}

void Core::Application::LoadProject(const std::string& filename)
{
	std::ifstream fin(filename);
	if (fin.is_open())
	{
		YAML::Node root = YAML::Load(fin);

		if (root["Project"])
		{
			root["Project"] >> m_Project;
		}

		fin.close();

		m_Window->SetTitle(m_Project.m_Name);
	}
	else
	{
		LOG_ERROR("Failed to open ", filename, " project file");
	}
}