#include "Camera.h"
#include "WindowEvents.h"

namespace Core::Gfx
{
    void Camera::OnEvent(Events::Event& event)
    {
        if (event.GetType() == "WindowResizedEvent")
        {
            auto& e = static_cast<Core::Events::WindowResizedEvent&>(event);
            float aspectRatio = (float)e.width / e.height;
            projection = glm::perspective(glm::radians(75.0f), aspectRatio, 0.1f, 1000.0f);
        }
    }

    float Camera::CalculateHorizontalDistance()
    {
        return distanceFromTarget * glm::cos(glm::radians(pitch));
    }

    float Camera::CalculateVerticalDistance() 
    {
        return distanceFromTarget * glm::sin(glm::radians(pitch));
    }

    void Camera::CalculateZoom()
    {
        accumulatedScroll -= Core::Input::Mouse::GetScrollOffset();
        accumulatedScroll = glm::max(0.4f, accumulatedScroll);
        distanceFromTarget = glm::mix(distanceFromTarget, accumulatedScroll, 9.0f * Window::GetDeltaTime());
    }

    // Default constructor
    Camera::Camera()
    {
        projection = glm::mat4(0.0f);
        Events::Dispatcher::Subscribe(std::bind(&Camera::OnEvent, this, std::placeholders::_1));
    }

    Camera::Camera(float aspectRatio)
    {
        projection = glm::perspective(glm::radians(75.0f), aspectRatio, 0.1f, 1000.0f);
    }

    void Camera::Move()
    {
        ProcessMouseInput();
        ProcessKeyboardInput();
        CalculateZoom();

        float horizontalDistance = CalculateHorizontalDistance();
        float verticalDistance = CalculateVerticalDistance();

        float xOffset = horizontalDistance * glm::sin(glm::radians(angleAroundTarget));
        float zOffset = horizontalDistance * glm::cos(glm::radians(angleAroundTarget));

        position.x = target.x - xOffset;
        position.z = target.z - zOffset;
        position.y = target.y + verticalDistance;

        // Clamp pitch within a reasonable range
        pitch = glm::clamp(pitch, -89.0f, 89.0f);

        yaw = 180.0f - (angleAroundTarget + glm::degrees(target.y));
    }

    void Camera::ProcessKeyboardInput()
    {
        using namespace Input;
        const float speed = 5.0f * Window::GetDeltaTime();

        // Get the forward and right vectors based on camera orientation
        glm::vec3 forward = glm::normalize(glm::vec3(target.x, 0.0f, target.z) - glm::vec3(position.x, 0.0f, position.z));
        glm::vec3 right = glm::normalize(glm::cross(forward, cameraUp));

        // Keyboard input for camera movement
        if (Keyboard::IsKeyDown(GLFW_KEY_W))
        {
            target += speed * forward;
        }
        if (Keyboard::IsKeyDown(GLFW_KEY_S))
        {
            target -= speed * forward;
        }
        if (Keyboard::IsKeyDown(GLFW_KEY_A))
        {
            target -= speed * right;
        }
        if (Keyboard::IsKeyDown(GLFW_KEY_D))
        {
            target += speed * right;
        }
    }

    void Camera::ProcessMouseInput()
    {
        // Get the forward and right vectors based on camera orientation
        glm::vec3 forward = glm::normalize(glm::vec3(target.x, 0.0f, target.z) - glm::vec3(position.x, 0.0f, position.z));
        glm::vec3 right = glm::normalize(glm::cross(forward, cameraUp));

        // Check if the right mouse button is pressed
        if (Core::Input::Mouse::IsButtonPressed(GLFW_MOUSE_BUTTON_RIGHT))
        {
            // Adjust the panning speed as needed
            const float panSpeed = distanceFromTarget * Window::GetDeltaTime();
            glm::vec2 mouseDelta = Core::Input::Mouse::GetMouseDelta();

            // Translate the target based on mouse movement
            target -= panSpeed * mouseDelta.x * right;
            target += panSpeed * mouseDelta.y * cameraUp;
        }

        // Handle rotation as before
        if (Core::Input::Mouse::IsButtonPressed(0))
        {
            glm::vec2 mouseDelta = Core::Input::Mouse::GetMouseDelta();
            float pitchChange = mouseDelta.y * 0.1f;
            pitch += pitchChange;

            float angleChange = mouseDelta.x * 0.3f;
            angleAroundTarget -= angleChange;
        }
    }

}