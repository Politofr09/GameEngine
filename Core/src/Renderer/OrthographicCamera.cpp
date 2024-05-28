#include "OrthographicCamera.h"
#include "Events/WindowEvents.h"

namespace Core::Gfx
{
	OrthographicCamera::OrthographicCamera()
	{
		m_ProjectionMatrix	= glm::mat4(1.0f);
		m_ViewMatrix		= glm::mat4(1.0f);
		m_Position			= glm::vec3(0.0f);
		m_Rotation			= 0.0f;

		Events::Dispatcher::Subscribe(std::bind(&OrthographicCamera::OnEvent, this, std::placeholders::_1));
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

	void OrthographicCamera::OnEvent(Events::Event& event)
	{
		if (event.GetType() == "WindowResizedEvent")
		{
			auto& e = static_cast<Core::Events::WindowResizedEvent&>(event);
			SetProjection(0.0f, e.width, e.height, 0.0f);
		}
	}

	void OrthographicCamera::CalculateViewMatrix()
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position) *
			glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), glm::vec3(0, 0, 1));

		m_ViewMatrix = glm::inverse(transform);
	}

}