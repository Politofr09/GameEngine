#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Core/Mouse.h"
#include "Core/Keyboard.h"
#include "Core/Window.h"
#include "Events/Event.h"

namespace Core::Gfx
{

    class Camera
    {
    private:
        glm::vec3 position = glm::vec3(0.0f, 20.0f, 0.0f);
        glm::vec3 target = glm::vec3(0.0f);
        glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
        glm::mat4 projection = glm::mat4(0.0f);

        float distanceFromTarget = 15.0f;
        float angleAroundTarget = 45.0f;
        float accumulatedScroll = distanceFromTarget;
        float pitch = 45.0f;
        float yaw = 45.0f; // Initialize yaw angle

    public:
        Camera();

        void Move();

        glm::mat4 GetViewMatrix() { return glm::lookAt(position, target, cameraUp); }

        glm::mat4 GetProjectionMatrix() { return projection; }
        void SetProjectionMatrix(glm::mat4 proj) { projection = proj; }
        void SetAspectRatio(float aspectRatio);
        float GetFOV() { return 2.0f * glm::atan(1.0f / projection[1][1]); }

        void OnEvent(Events::Event* event);

    private:
        float CalculateHorizontalDistance();
        float CalculateVerticalDistance();
        void CalculateZoom();
        void ProcessMouseInput();
        void ProcessKeyboardInput();
    };

}