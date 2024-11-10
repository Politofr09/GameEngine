#include "OrthographicCamera.h"
#include "FrameBuffer.h"
#include "Core/Mouse.h"
#include "Core/Keyboard.h"
#include "Core/Window.h"

namespace Core::Gfx
{
	OrthographicCamera::OrthographicCamera()
	{
		m_ProjectionMatrix	= glm::mat4(1.0f);
		m_ViewMatrix		= glm::mat4(1.0f);
	}

	OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
	{
		m_Right = right;
		m_Bottom = bottom;
		SetProjection(left, right, bottom, top);
		CalculateViewMatrix();
	}

	void OrthographicCamera::SetProjection(float left, float right, float bottom, float top)
	{
		m_Right = right;
		m_Bottom = bottom;
		m_ProjectionMatrix = glm::ortho(left / m_Zoom, right / m_Zoom, bottom / m_Zoom, top / m_Zoom, -100.0f, 100.0f);
	}

	void OrthographicCamera::OnViewportResize(int width, int height)
	{
		SetProjection(0.0f, width, height, 0.0f);
	}

	void OrthographicCamera::CalculateViewMatrix()
	{
		glm::mat4 matTranslation = glm::translate(glm::mat4(1.0f), -m_Position);
		glm::mat4 matRotation = glm::rotate(glm::mat4(1.0f), glm::radians(-m_Rotation), glm::vec3(0.0f, 0.0f, 1.0f));

		m_ViewMatrix = matRotation * matTranslation;
	}

	void OrthographicCamera::Move()
	{
		using namespace Core::Input;

		float cameraSpeed = 10.0f * Window::GetDeltaTime(); // Adjust speed as needed

		if (Mouse::IsButtonPressed(GLFW_MOUSE_BUTTON_LEFT))
		{
			glm::vec2 delta = Mouse::GetMouseDelta();
			m_Position.x -= delta.x;
			m_Position.y -= delta.y;
		}

		m_Zoom += Mouse::GetScrollOffset() * Window::GetDeltaTime() * 10.0f;
		m_Zoom = glm::clamp(m_Zoom, 0.001f, 10.0f);
		
		SetProjection(0, m_Right, m_Bottom, 0);
		CalculateViewMatrix();
	}

}