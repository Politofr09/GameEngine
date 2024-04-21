#include <GLFW/glfw3.h>
#include <string>
#include <chrono>

#pragma once

namespace Core
{
	class Window
	{
	private:
		uint32_t _width;
		uint32_t _height;
		std::string _title;

		GLFWwindow* _window; // GLFW Handle

		static std::chrono::steady_clock::time_point _prevTime;
		static float _deltaTime;

		static void window_resize_callback(GLFWwindow* window, int width, int height);
		static void window_pos_callback(GLFWwindow* window, int xpos, int ypos);

		void Init();

	public:
		Window(uint32_t w, uint32_t h, const std::string& title = "");

		uint32_t GetWidth();
		uint32_t GetHeight();

		// Render what's on the screen
		void Update();

		bool ShouldClose();

		void Close();

		static float GetDeltaTime() { return _deltaTime; }

		GLFWwindow* GetHandle() { return _window; }
	};
}