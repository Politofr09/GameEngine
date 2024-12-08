#include "Renderer2D.h"
#include "Core/Utils.h"
#include "Core/Application.h"

namespace Core::Gfx
{

	struct QuadVertex
	{
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 TexCoord;
		float	  TexIndex;
	};

	struct Renderer2DData
	{
		static const uint32_t MaxQuads = 20000;
		static const uint32_t MaxVertices = MaxQuads * 4;
		static const uint32_t MaxIndices = MaxQuads * 6;

		/* 31 usable because of white dummy pixel texture
		 * OpenGL-compatible gpus need to provide at least 16 texture slots per shader call.
		 * In the worst case we have 15 usable slots.
		**********************************************************************************/
		static const uint32_t MaxTextureSlots = 32;
		std::array<Ref<Texture>, MaxTextureSlots> TextureSlots;
		uint32_t TextureSlotIndex = 1; // 0 for pixel dummy texture

		OrthographicCamera ActiveCamera;

		Shader QuadShader;

		VertexArray QuadVertexArray;
		VertexBuffer QuadVertexBuffer;

		Ref<Texture> PixelDummy; // When drawing only shapes we multiply a white pixel just for having the same shader

		uint32_t QuadIndexCount = 0;
		QuadVertex* QuadVertexBufferBase = nullptr;
		QuadVertex* QuadVertexBufferPtr = nullptr;
	};

	static Renderer2DData s_Data;

	void Renderer2D::Init()
	{
		// TODO: Load this from the disk idk why I put it  here... 
		// Make the shader
		const char* vertexShaderSource = R"(
			#version 450 core
			layout (location = 0) in vec3 a_Position;
			layout (location = 1) in vec4 a_Color;
			layout (location = 2) in vec2 a_TexCoord;
			layout (location = 3) in float a_TexIndex;

			out vec2 v_TexCoord;
			out vec4 v_Color;
			out float v_TexIndex;

			uniform mat4 u_Projection;
			uniform mat4 u_View;

			void main()
			{
				v_TexCoord = a_TexCoord;
				v_Color = a_Color;
				v_TexIndex = a_TexIndex;

				vec4 scaledPosition = vec4(a_Position, 1.0);
				gl_Position = u_Projection * u_View * scaledPosition;
			}
		)";

		const char* fragmentShaderSource = R"(
			#version 450 core
			in vec2 v_TexCoord;
			in vec4 v_Color;
			in float v_TexIndex;

			out vec4 color;
			
			layout (binding = 0) uniform sampler2D u_Textures[32];

			void main()
			{
				color = texture(u_Textures[int(v_TexIndex)], v_TexCoord) * vec4(v_Color);
			}
		)";

		s_Data.QuadShader.LoadFromMemory(vertexShaderSource, fragmentShaderSource);

		// Vertex buffer setup
		s_Data.QuadVertexArray.Init();
		BufferLayout layout;
		layout.Push<float>(3); // Position
		layout.Push<float>(4); // Color
		layout.Push<float>(2); // TexCoords
		layout.Push<float>(1); // TexIndex

		s_Data.QuadVertexBuffer = VertexBuffer(s_Data.MaxIndices * sizeof(QuadVertex));
		s_Data.QuadVertexArray.AddBuffer(s_Data.QuadVertexBuffer, layout);

		s_Data.QuadVertexBufferBase = new QuadVertex[s_Data.MaxVertices];

		unsigned int* quadIndices = new unsigned int[s_Data.MaxIndices];

		unsigned int offset = 0;
		for (unsigned int i = 0; i < s_Data.MaxIndices; i += 6)
		{
			quadIndices[i + 0] = offset + 0;
			quadIndices[i + 1] = offset + 1;
			quadIndices[i + 2] = offset + 2;

			quadIndices[i + 3] = offset + 2;
			quadIndices[i + 4] = offset + 3;
			quadIndices[i + 5] = offset + 0;

			offset += 4;
		}
		IndexBuffer ib(quadIndices, s_Data.MaxIndices);
		s_Data.QuadVertexArray.SetIndexBuffer(ib);
		delete[] quadIndices; // Warning: not thread safe 

		// Init dummy pixel white texture
		unsigned int whitePixel = 0xFFFFFF;
		unsigned char* whitePixelPtr = reinterpret_cast<unsigned char*>(&whitePixel);
		s_Data.PixelDummy = Texture::CreateFromMemory(1, 1, 3, whitePixelPtr);
		s_Data.TextureSlots[0] = s_Data.PixelDummy;
	}

	void Renderer2D::Begin(const OrthographicCamera& camera)
	{
		s_Data.ActiveCamera = camera;

		s_Data.QuadShader.Use();

		s_Data.QuadShader.SetMatrix("u_Projection", s_Data.ActiveCamera.GetProjectionMatrix());
		s_Data.QuadShader.SetMatrix("u_View", s_Data.ActiveCamera.GetViewMatrix());

		StartBatch();

		glDisable(GL_CULL_FACE);
	}

	void Renderer2D::StartBatch()
	{
		s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;
		s_Data.QuadIndexCount = 0;
		s_Data.TextureSlotIndex = 1;

		for (uint32_t i = 0; i < Renderer2DData::MaxTextureSlots; i++)
		{
			s_Data.TextureSlots[i] = s_Data.PixelDummy;
		}
	}

	void Renderer2D::NextBatch()
	{
		Flush();
		StartBatch();
	}

	void Renderer2D::End()
	{
		Flush();
	}

	void Renderer2D::Flush()
	{
		s_Data.QuadVertexArray.Bind();
		size_t dataSize = (uint8_t*)s_Data.QuadVertexBufferPtr - (uint8_t*)s_Data.QuadVertexBufferBase;
		s_Data.QuadVertexBuffer.UpdateData(s_Data.QuadVertexBufferBase, dataSize);

		// Bind textures
		for (uint32_t i = 0; i <= s_Data.TextureSlotIndex; i++)
		{
			s_Data.TextureSlots[i]->Bind(i);
		}

		// Execute draw call
		glDrawElements(GL_TRIANGLES, s_Data.QuadIndexCount, GL_UNSIGNED_INT, nullptr);
	}

	void Renderer2D::DrawQuad(glm::vec3& position, glm::vec2& size, glm::vec4& color)
	{
		if (s_Data.QuadIndexCount + 6 > s_Data.MaxIndices ||
			(s_Data.QuadVertexBufferPtr - s_Data.QuadVertexBufferBase) + 4 > s_Data.MaxVertices)
		{
			NextBatch();
		}

		s_Data.QuadVertexBufferPtr->Position = position;
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 0.0f, 0.0f };
		s_Data.QuadVertexBufferPtr->TexIndex = 0.0f;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = { position.x + size.x, position.y, position.z };
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 1.0f, 0.0f };
		s_Data.QuadVertexBufferPtr->TexIndex = 0.0f;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = { position.x + size.x, position.y + size.y, position.z };
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 1.0f, 1.0f };
		s_Data.QuadVertexBufferPtr->TexIndex = 0.0f;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = { position.x, position.y + size.y, position.z };
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 0.0f, 1.0f };
		s_Data.QuadVertexBufferPtr->TexIndex = 0.0f;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadIndexCount += 6;
	}

	void Renderer2D::DrawQuad(glm::vec2& position, glm::vec2& size, glm::vec4& color)
	{
		DrawQuad(glm::vec3{ position.x, position.y, 0.0f }, size, color);
	}

	void Renderer2D::DrawQuadTextured(glm::vec3& position, glm::vec2& size, glm::vec4& color, Ref<Texture> texture)
	{
		// Check if the batch should be restarted based on index limits
		if (s_Data.QuadIndexCount + 6 > s_Data.MaxIndices ||
			(s_Data.QuadVertexBufferPtr - s_Data.QuadVertexBufferBase) + 4 > s_Data.MaxVertices)
		{
			NextBatch();
		}

		// Check if texture is already bound, if not add it
		float texIndex = 0.0f;
		for (uint32_t i = 0; i <= s_Data.TextureSlotIndex; i++)
		{
			if (s_Data.TextureSlots[i]->GetID() == texture->GetID())
			{
				texIndex = (float)i;
				break;
			}
		}

		if (texIndex == 0.0f)
		{
			if (s_Data.TextureSlotIndex >= Renderer2DData::MaxTextureSlots)
			{
				NextBatch();
			}

			// Add the texture to the slots
			s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
			texIndex = (float)s_Data.TextureSlotIndex;
			s_Data.TextureSlotIndex++;
		}

		// Define quad vertices
		s_Data.QuadVertexBufferPtr->Position = position;
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 0.0f, 0.0f };
		s_Data.QuadVertexBufferPtr->TexIndex = texIndex;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = { position.x + size.x, position.y, position.z };
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 1.0f, 0.0f };
		s_Data.QuadVertexBufferPtr->TexIndex = texIndex;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = { position.x + size.x, position.y + size.y, position.z };
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 1.0f, 1.0f };
		s_Data.QuadVertexBufferPtr->TexIndex = texIndex;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = { position.x, position.y + size.y, position.z };
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 0.0f, 1.0f };
		s_Data.QuadVertexBufferPtr->TexIndex = texIndex;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadIndexCount += 6;
	}

}