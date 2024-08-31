#include "Renderer2D.h"
#include "Core/Utils.h"

namespace Core::Gfx
{

	// Resolve external symbols
	OrthographicCamera Renderer2D::m_ActiveCamera;
	Shader Renderer2D::m_Shader;

	VertexArray  Renderer2D::m_QuadVAO;
	VertexBuffer Renderer2D::m_QuadVBO;

	Texture Renderer2D::m_PixelDummy;

	void Renderer2D::Init()
	{
		// Make the shader
		const char* vertexShaderSource = R"(
			#version 330 core
			layout (location = 0) in vec2 a_Position;

			out vec2 TexCoord;

			uniform mat4 uTransform;
			uniform mat4 uProjection;
			uniform mat4 uView;

			uniform vec2 uTexCoordOffset;
			uniform vec2 uTexCoordScaling;
 
			void main()
			{
				TexCoord = a_Position;
				TexCoord += uTexCoordOffset;
				TexCoord *= uTexCoordScaling;
				gl_Position = uProjection * uView * uTransform * vec4(a_Position, 0.0, 1.0);
			}
		)";

		const char* fragmentShaderSource = R"(
			#version 330 core
			in vec2 TexCoord;
			out vec4 color;
			
			uniform sampler2D image;
			uniform vec3 uColor;

			void main()
			{
				color = texture(image, TexCoord) * vec4(uColor, 1.0);
			}
		)";

		m_Shader.LoadFromMemory(vertexShaderSource, fragmentShaderSource);

		m_QuadVAO.Init();
		float vertices[] = {
			// Positions
			0.0f, 1.0f,
			1.0f, 0.0f,
			0.0f, 0.0f,

			0.0f, 1.0f,
			1.0f, 1.0f,
			1.0f, 0.0f,		
			
		};

		BufferLayout quad_layout;
		quad_layout.Push<float>(2);
		m_QuadVBO = VertexBuffer(vertices, sizeof(vertices));
		m_QuadVAO.AddBuffer(m_QuadVBO, quad_layout);

		// Init dummy pixel white texture
		unsigned int whitePixel = 0xFFFFFF;
		unsigned char* whitePixelPtr = reinterpret_cast<unsigned char*>(&whitePixel);
		m_PixelDummy.LoadFromMemory(1, 1, 3, whitePixelPtr);
	}

	void Renderer2D::SendUniforms(glm::mat4 transform, glm::vec3 tint,  glm::vec2 uvOffset, glm::vec2 uvScaling)
	{
		m_Shader.SetMatrix("uProjection", m_ActiveCamera.GetProjectionMatrix());
		m_Shader.SetMatrix("uView", m_ActiveCamera.GetViewMatrix());
		m_Shader.SetMatrix("uTransform", transform);
		m_Shader.SetVector3("uColor", tint);
		m_Shader.SetVector2("uTexCoordOffset", uvOffset);
		m_Shader.SetVector2("uTexCoordScaling", uvScaling);
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
		
		// Draw quad vertex buffer
		m_QuadVAO.Bind();
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

    void Renderer2D::DrawTextureRect(Texture &texture, glm::vec2 position, glm::vec4 rectangle, glm::vec2 scale, float rotation, glm::vec3 tint)
    {
		glm::mat4 transform = glm::mat4(1.0f);
		transform = glm::translate(transform, glm::vec3(position, 0.0f));
		transform = glm::rotate(transform, glm::radians(rotation), glm::vec3(0, 0, 1.0f));
		transform = glm::scale(transform, glm::vec3(scale * 100.0f, 1.0f));

		SendUniforms(transform, tint, { rectangle.x / texture.GetWidth(), rectangle.y / texture.GetHeight() }, { rectangle.z / texture.GetWidth(), rectangle.a / texture.GetHeight() });
		
		// Bind texture
		texture.Bind();
		glEnable(GL_TEXTURE_2D);
		
		// Draw quad vertex buffer
		m_QuadVAO.Bind();
		glDrawArrays(GL_TRIANGLES, 0, 6);
    }

    void Renderer2D::DrawRectangle(glm::vec2 position, glm::vec2 size, glm::vec3 color)
	{
		glm::mat4 transform = glm::mat4(1.0f);
		transform = glm::translate(transform, glm::vec3(position, 0.0f));
		transform = glm::scale(transform, glm::vec3(size, 1.0f));

		SendUniforms(transform, color);
		
		// Bound the dummy white texture
		m_PixelDummy.Bind();

		// Draw quad vertex buffer
		m_QuadVAO.Bind();
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	void Renderer2D::DrawLine(glm::vec2 start, glm::vec2 end, glm::vec3 color)
	{
		SendUniforms(glm::mat4(1.0f), color);

		// Make sure no texture is bound
		m_PixelDummy.Bind();

		glBegin(GL_LINES);
			glVertex2f(start.x, start.y);
			glVertex2f(end.x, end.y);
		glEnd();
	}

    void Renderer2D::DrawPolygon(const std::vector<float> &vertices, glm::vec2 position, glm::vec2 scale, glm::vec3 color)
    {
		// Transform uniform setup
		glm::mat4 transform(1.0f);
		transform = glm::translate(transform, glm::vec3(position, 0.0f));
		transform = glm::scale(transform, glm::vec3(scale, 0.0f));

		SendUniforms(transform, color);

		m_PixelDummy.Bind();

		glBegin(GL_POLYGON);

		for (int i = 0; i < vertices.size(); i+=2)
		{
			glVertex2f(vertices[i], vertices[i + 1]);
		}

		glEnd();
	}
    std::vector<float> Renderer2D::GeneratePolygonVertices(int numSides, float radius)
	{
		std::vector<float> vertices;
		float angleStep = glm::radians(360.0f / numSides);

		for (int i = 0; i < numSides; ++i)
		{
			float angle = i * angleStep;
			float x = radius * cos(angle);
			float y = radius * sin(angle);
			vertices.push_back(x);
			vertices.push_back(y);
		}

		return vertices;
	}
}