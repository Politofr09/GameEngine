#pragma once 

#include "Core/Layer.h"
#include "Core/Window.h"  
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

namespace Core
{


	// TODO: Add imgui events
	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer() : Layer("ImGuiLayer") {}

		// Function accessible so anyone can change the theme
		static void SwitchTheme(bool dark);
		inline static bool IsDarkThemed() { return m_Dark; }

		void SetupImGui(bool dark = true);

		// When set to true, all input events will be blocked to imgui
		void BlockEvents(bool block) { m_BlockEvents = block; };

		void OnAttach() override;
		void OnUpdate() override;
		void OnImGuiRender() override;
		void OnEvent(Events::Event* event) override;
		void OnDettach() override;

		void BeginImGuiContent(); // Call before any imgui code inside main loop
		void EndImGuiContent();   // Call after any imgui code inside main loop

		~ImGuiLayer();

	private:
		static bool m_Dark;
		static bool m_BlockEvents;
	};

}