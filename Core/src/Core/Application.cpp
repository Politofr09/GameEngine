#include "Application.h"

Core::Application::Application(const std::string& name, int window_width, int window_height)
{
	m_Window = new Window(window_width, window_height, name);
	m_Name = name;

	m_ImGuiLayer = new ImGuiLayer(m_Window);
	m_ImGuiLayer->SetupImGui(m_Window);

	Events::Dispatcher::Subscribe(std::bind(&ImGuiLayer::OnEvent, m_ImGuiLayer, std::placeholders::_1));
	Events::Dispatcher::Subscribe(std::bind(&Application::OnEvent, this, std::placeholders::_1));
}

void Core::Application::PushLayer(Layer* layer)
{
	layer->OnAttach();
	// Subscribe layer to all events
	Events::Dispatcher::Subscribe(std::bind(&Layer::OnEvent, layer, std::placeholders::_1));
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

		m_Window->Update();
	}
}

void Core::Application::OnEvent(Core::Events::Event& event)
{
}

Core::Application::~Application()
{
	delete m_ImGuiLayer;
	m_Window->Close();

	for (auto& layer : m_Layers)
	{
		layer->OnDettach();
		delete layer;
	}
	m_Layers.clear();
}
