#include <GLFW/glfw3.h>
#include <string>

#pragma once

namespace Core
{
	class Window
	{
	private:
		uint32_t _width;
		uint32_t _height;
		std::string _title;

		GLFWwindow* _window;

		void Init();

	public:
		Window(uint32_t w, uint32_t h, const std::string& title = "");

		uint32_t GetWidth();
		uint32_t GetHeight();

		// Render what's on the screen
		void Update();

		bool ShouldClose();

		void Close();

		GLFWwindow* GetHandle() { return _window; }
	};
}