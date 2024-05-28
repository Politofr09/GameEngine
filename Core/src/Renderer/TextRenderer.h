#include "Font.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "Shader.h"
#include "OrthographicCamera.h"

#pragma once

namespace Core::Gfx
{

	class TextRenderer
	{
	public:
		static void Init();

		static void Begin(const OrthographicCamera& camera);
		static void End();

		static void DrawText(const std::string& text, float x, float y, float scale, glm::vec3 tint);
		static void DrawTextEx(Font& font, const std::string& text, float x, float y, float scale, glm::vec3 tint);


	private:
		static VertexBuffer m_VB;
		static VertexArray m_VA;
		static Shader m_Shader;
		static OrthographicCamera m_ActiveCamera;
		static Font m_DefaultFont;

	};

}