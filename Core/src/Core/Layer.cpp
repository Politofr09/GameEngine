#include "Layer.h"
#include "Application.h"

Core::Layer::Layer(Application* app, const std::string& name)
{
	m_Application = app;
	m_Name = name;
}
