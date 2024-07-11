#pragma once 

#include "Layer.h"
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
		ImGuiLayer(Window* window) : Layer(window) {}

		// Function accessible so anyone can change the theme
		static void SwitchTheme(bool dark);
		inline static bool IsDarkThemed() { return m_Dark; }

		// Is the user controlling the game viewport or just inside the ImGui ui?
		inline static bool OnTop() { return m_OnTop; }
		
		void SetupImGui(Window* window, bool dark = true, bool useRobotoFont = false);

		// Placeholder methods just to override the virtual functions of 'Layer'
		void OnAttach() override;
		void OnUpdate() override;
		void OnImGuiRender() override;
		void OnEvent(Core::Events::Event* event) override;
		void OnDettach() override;

		void BeginImGuiContent(); // Call before any imgui code inside main loop
		void EndImGuiContent();   // Call after any imgui code inside main loop

		~ImGuiLayer();

		// Extra ImGui utilities
		static void DisplayRoundedImage(ImTextureID texture_id, ImVec2 size, float rounding, const ImVec2& uv0 = ImVec2(0, 0), const ImVec2& uv1 = ImVec2(1, 1), ImVec4 color = ImVec4(1, 1, 1, 1));
		static bool IsMouseHoveringTitleBar();

	private:
		static bool m_Dark;
		static bool m_OnTop; // Are we using this layer or the Sandbox layer?
	};
}