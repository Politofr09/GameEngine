#include "VertexArray.h"
#include "VertexBuffer.h"
#include "Texture.h"
#include "Shader.h"
#include "OrthographicCamera.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#pragma once

namespace Core::Gfx
{
	// TODO: Add more shapes; be able to use custom shader
	class Renderer2D
	{
	public:
		// Initialize projection & VAO render data
		static void Init();

		static void Begin(const OrthographicCamera& camera);
		static void End();

		static void DrawTexture(Texture& texture, glm::vec2 position, glm::vec2 scale = glm::vec2(1, 1), float rotation = 0.0f, glm::vec3 tint = glm::vec3(1, 1, 1));
		static void DrawTextureRect(Texture& texture, glm::vec2 position, glm::vec4 rectangle, glm::vec2 scale = glm::vec2(1, 1), float rotation = 0.0f, glm::vec3 tint = glm::vec3(1, 1, 1));
		static void DrawRectangle(glm::vec2 position, glm::vec2 size, glm::vec3 color);
		static void DrawLine(glm::vec2 start, glm::vec2 end, glm::vec3 color);
		static void DrawPolygon(const std::vector<float>& vertices, glm::vec2 position, glm::vec2 scale, glm::vec3 color);

		static std::vector<float> GeneratePolygonVertices(int numSides, float radius = 1.0f);

	private:
		static void SendUniforms(glm::mat4 transform, glm::vec3 tint, glm::vec2 uvOffset = glm::vec2(0.0f), glm::vec2 uvScaling = glm::vec2(1.0f, 1.0f));

	private:
		static OrthographicCamera m_ActiveCamera;
		static Shader m_Shader;

		static VertexArray m_QuadVAO;
		static VertexBuffer m_QuadVBO;

		static Texture m_PixelDummy; // When drawing only shapes we multiply a white pixel just for having the same shader
	};
	
}