#include "OrthographicCamera.h"
#include "FrameBuffer.h"

namespace Core::Gfx
{
	OrthographicCamera::OrthographicCamera()
	{
		m_ProjectionMatrix	= glm::mat4(1.0f);
		m_ViewMatrix		= glm::mat4(1.0f);
		m_Position			= glm::vec3(0.0f);
		m_Rotation			= 0.0f;
	}

	OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
	{
		SetProjection(left, right, bottom, top);
		CalculateViewMatrix();
	}

	void OrthographicCamera::SetProjection(float left, float right, float bottom, float top)
	{
		m_ProjectionMatrix = glm::ortho(left, right, bottom, top);
	}

	void OrthographicCamera::OnViewportResize(int width, int height)
	{
		SetProjection(0.0f, width, height, 0.0f);
	}

	void OrthographicCamera::CalculateViewMatrix()
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position) *
			glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), glm::vec3(0, 0, 1));

		m_ViewMatrix = glm::inverse(transform);
	}

}