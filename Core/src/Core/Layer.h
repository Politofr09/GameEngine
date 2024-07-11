#include <string>
#include "Events/Event.h"
#include "Core/Window.h"

#pragma once

namespace Core
{
	// Forward declaration
	class Application;

	class Layer
	{
	public:
		
		Layer(Window* window, const std::string& name = "Layer");

		virtual void OnAttach() = 0;
		virtual void OnUpdate() = 0;
		virtual void OnImGuiRender() = 0;
		///*IMPORTANT: BEFORE CASTING EVENT WITH static_cast CHECK IF IT'S MATCHING THE EXPECTED TYPE
		/// Event.GetType() = "myExpectedEvent" AND THEN CAST.
		virtual void OnEvent(Events::Event* event) = 0;
		virtual void OnDettach() = 0;

		const std::string& GetName() { return m_Name; }

		virtual ~Layer() = default;

	private:
		std::string m_Name;

	protected:
		Window* m_Window;
	};
}