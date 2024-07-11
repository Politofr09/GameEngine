#include "Renderer2D.h"
#include "Core/Utils.h"

namespace Core::Gfx
{

	// Resolve external symbols
	OrthographicCamera Renderer2D::m_ActiveCamera;
	Shader Renderer2D::m_Shader;

	void Renderer2D::Init()
	{
		// Make the shader
		const char* vertexShaderSource = R"(
			#version 330 core
			layout (location = 0) in vec2 vertex; // vec2 position (and uv = position)

			out vec2 uv;

			uniform mat4 uTransform;
			uniform mat4 uProjection;
			uniform mat4 uView;
 
			void main()
			{
				uv = vertex;
				gl_Position = uProjection * uView * uTransform * vec4(vertex, 0.0, 1.0);
			}
		)";

		const char* fragmentShaderSource = R"(
			#version 330 core
			in vec2 uv;
			out vec4 color;
			
			uniform sampler2D image;
			uniform vec3 uColor;
			uniform int isText;

			void main()
			{
				color = vec4(uColor, 1.0) * texture(image, uv);
			}
		)";

		m_Shader.LoadFromMemory(vertexShaderSource, fragmentShaderSource);
	}

	void Renderer2D::SendUniforms(glm::mat4 transform, glm::vec3 tint)
	{
		m_Shader.SetMatrix("uProjection", m_ActiveCamera.GetProjectionMatrix());
		m_Shader.SetMatrix("uView", m_ActiveCamera.GetViewMatrix());
		m_Shader.SetMatrix("uTransform", transform);
		m_Shader.SetVector3("uColor", tint);
	}

	void Renderer2D::Begin(const OrthographicCamera& camera)
	{
		m_ActiveCamera = camera;
		m_Shader.Use();

		// Get the projection matrix from the camera
		const glm::mat4& projectionMatrix = camera.GetProjectionMatrix();

		float bottom = (1 - projectionMatrix[1][3]) / projectionMatrix[1][1];
		float top = -(1 + projectionMatrix[1][3]) / projectionMatrix[1][1];
		float left = -(1 + projectionMatrix[0][3]) / projectionMatrix[0][0];
		float right = (1 - projectionMatrix[0][3]) / projectionMatrix[0][0];

		float window_width = abs(right - left);
		float window_height = abs(top - bottom);

		glViewport(0, 0, window_width, window_height);
	}

	void Renderer2D::End()
	{
	}

	void Renderer2D::DrawTexture(Texture& texture, glm::vec2 position, glm::vec2 scale, float rotation, glm::vec3 tint)
	{
		// Prepare transformation matrix
		glm::mat4 transform = glm::mat4(1.0f);
		transform = glm::translate(transform, glm::vec3(position, 0.0f));
		transform = glm::rotate(transform, glm::radians(rotation), glm::vec3(0, 0, 1.0f));
		transform = glm::scale(transform, glm::vec3(scale * 100.0f, 1.0f));

		// Set shader's uniforms
		SendUniforms(transform, tint);

		// Bind texture
		texture.Bind();
		glEnable(GL_TEXTURE_2D);
		
		glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 1.0f); glVertex2f(0.0f, 0.0f);
			glTexCoord2f(1.0f, 1.0f); glVertex2f(1.0f, 0.0f);
			glTexCoord2f(1.0f, 0.0f); glVertex2f(1.0f, 1.0f);
			glTexCoord2f(0.0f, 0.0f); glVertex2f(0.0f, 1.0f);
		glEnd();
	}

	void Renderer2D::DrawRectangle(glm::vec2 position, glm::vec2 size, glm::vec3 color)
	{
		glm::mat4 transform = glm::mat4(1.0f);
		transform = glm::translate(transform, glm::vec3(position, 0.0f));
		transform = glm::scale(transform, glm::vec3(size, 1.0f));

		SendUniforms(transform, color);
		
		// Make sure no texture is bound
		glBindTexture(GL_TEXTURE_2D, 0);

		glBegin(GL_QUADS);
			glVertex2f(0.0f, 0.0f);
			glVertex2f(1.0f, 0.0f);
			glVertex2f(1.0f, 1.0f);
			glVertex2f(0.0f, 1.0f);
		glEnd();
	}

	void Renderer2D::DrawLine(glm::vec2 start, glm::vec2 end, glm::vec3 color)
	{
		SendUniforms(glm::mat4(1.0f), color);

		// Make sure no texture is bound
		glBindTexture(GL_TEXTURE_2D, 0);

		glBegin(GL_LINES);
			glVertex2f(start.x, start.y);
			glVertex2f(end.x, end.y);
		glEnd();
	}

}