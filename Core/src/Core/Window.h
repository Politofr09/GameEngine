#include <GLFW/glfw3.h>
#include <string>
#include <chrono>

#pragma once

namespace Core
{
	class Window
	{
	public:
		Window(uint32_t w, uint32_t h, const std::string& title = "");

		uint32_t GetWidth();
		uint32_t GetHeight();

		// Render what's on the screen
		void Update();

		bool ShouldClose();

		void Close();
		void Maximize();
		void Minimize();

		void SetTitle(const std::string& title);

		static float GetDeltaTime() { return m_DeltaTime; }

		GLFWwindow* GetHandle() { return m_Window; }

	private:
		uint32_t m_Width;
		uint32_t m_Height;
		std::string m_Title;

		GLFWwindow* m_Window; // GLFW Handle

		static std::chrono::steady_clock::time_point m_PrevTime;
		static float m_DeltaTime;

		static void window_resize_callback(GLFWwindow* window, int width, int height);
		static void window_pos_callback(GLFWwindow* window, int xpos, int ypos);

		void Init(bool decorated = true);

	};
}