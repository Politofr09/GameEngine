#include "TextRenderer.h"
#include "DefaultFont.h"

namespace Core::Gfx
{
	// Resolve external symbols
	OrthographicCamera TextRenderer::m_ActiveCamera;
	Shader TextRenderer::m_Shader;
	VertexArray TextRenderer::m_VA;
	VertexBuffer TextRenderer::m_VB;
	Font TextRenderer::m_DefaultFont;

	void TextRenderer::Init()
	{
		const char* vertexShaderSource = R"(
			#version 330 core
			layout (location = 0) in vec4 vertex; // vec2 position + vec2 uv
			out vec2 uv;

			uniform mat4 uProjection;
			uniform mat4 uView;
 
			void main()
			{
				gl_Position = uProjection * uView * vec4(vertex.xy, 0.0, 1.0);
				uv = vertex.zw;
			}
		)";

		const char* fragmentShaderSource = R"(
			#version 330 core
			in vec2 uv;
			out vec4 color;

			uniform sampler2D text;
			uniform vec3 uTextColor;

			void main()
			{    
				vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, uv).r);
				color = vec4(uTextColor, 1.0) * sampled;
				// color = vec4(1.0, 1.0, 1.0, 1.0);
			}
		)";

		m_Shader.LoadFromMemory(vertexShaderSource, fragmentShaderSource);

		// Setup buffers
		m_VB = VertexBuffer(nullptr, 6 * 4 * sizeof(float), true); // dynamic draw
		BufferLayout layout;
		layout.Push<float>(4);

		m_VA.AddBuffer(m_VB, layout);

		// Load default font
		m_DefaultFont.LoadFromMemory(OpenSans_Medium_ttf, OpenSans_Medium_ttf_len, 35);
	}

	void TextRenderer::Begin(const OrthographicCamera& camera)
	{
		m_ActiveCamera = camera;
		m_Shader.Use();
		m_Shader.SetMatrix("uProjection", m_ActiveCamera.GetProjectionMatrix());
		m_Shader.SetMatrix("uView", m_ActiveCamera.GetViewMatrix());
	}

	void TextRenderer::End()
	{
	}

	void TextRenderer::DrawText(const std::string& text, float x, float y, float scale, glm::vec3 tint)
	{
		DrawTextEx(m_DefaultFont, text, x, y, scale, tint);
	}

	void TextRenderer::DrawTextEx(Font& font, const std::string& text, float x, float y, float scale, glm::vec3 tint)
	{
		scale /= 15;
		m_Shader.SetVector3("uTextColor", tint);

		glActiveTexture(GL_TEXTURE0);
		m_VB.Bind();
		m_VA.Bind();

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		// Render glyph texture over quad
		glBindTexture(GL_TEXTURE_2D, font.GetTextureID());
		for (char c : text)
		{
			Character ch = font.GetCharacter(c);

			float xpos = x + ch.Bearing.x * scale;
			float ypos = y + ch.Bearing.y * scale;

			float w = ch.Size.x * scale;
			float h = ch.Size.y * scale;

			// Update VBO for each character
			float vertices[6][4] = {
				{ xpos,     ypos + h,   ch.TexCoords[0].x, ch.TexCoords[1].y },
				{ xpos,     ypos,       ch.TexCoords[0].x, ch.TexCoords[0].y },
				{ xpos + w, ypos,       ch.TexCoords[1].x, ch.TexCoords[0].y },

				{ xpos,     ypos + h,   ch.TexCoords[0].x, ch.TexCoords[1].y },
				{ xpos + w, ypos,       ch.TexCoords[1].x, ch.TexCoords[0].y },
				{ xpos + w, ypos + h,   ch.TexCoords[1].x, ch.TexCoords[1].y }
			};

			// Update content of VBO memory
			m_VB.UpdateData(vertices, sizeof(vertices));

			// Render quad
			glDrawArrays(GL_TRIANGLES, 0, 6);

			float offset = ch.Advance * scale;
			x += offset;
		}

		m_VA.UnBind();
		glBindTexture(GL_TEXTURE_2D, 0);
	}

}
