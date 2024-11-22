#include "VertexArray.h"
#include "VertexBuffer.h"
#include "Texture.h"
#include "Shader.h"
#include "OrthographicCamera.h"
#include "FrameBuffer.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#pragma once

namespace Core::Gfx
{
	class Renderer2D
	{
	public:
		// Initialize projection & VAO render data
		static void Init();

		static void Begin(const OrthographicCamera& camera);
		static void End();

		static void DrawQuad(glm::vec3& position, glm::vec2& size, glm::vec4& color);
		static void DrawQuad(glm::vec2& position, glm::vec2& size, glm::vec4& color);
		static void DrawQuadTextured(glm::vec3& position, glm::vec2& size, glm::vec4& color, const Texture& texture);

	private:
		static void StartBatch();
		static void NextBatch();
		static void Flush();

	private:
		static void SendUniforms();
	};
	
}