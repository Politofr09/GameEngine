#include <string>
#include "VertexArray.h"
#include "IndexBuffer.h"

#pragma once

namespace Core::Gfx
{

	// This class can only be initialized by Loader
	class Model
	{
	private:
		unsigned int m_RendererID;  // Internal OpenGL ID
		unsigned int m_VertexCount; // Also indices count

		IndexBuffer m_Ib;
		VertexBuffer m_Vb;

	public:
		Model() = default;

		Model(unsigned int vaoID, const IndexBuffer& ib, const VertexBuffer& vb)
			: m_RendererID(vaoID), m_Ib(ib), m_Vb(vb), m_VertexCount(ib.GetCount()) {}

		void Free()
		{
			m_Ib.Free();
			m_Vb.Free();
		}

		unsigned int GetRendererID()
		{
			return m_RendererID;
		}

		unsigned int GetVertexCount()
		{
			return m_VertexCount;
		}

		IndexBuffer GetIndexBuffer()
		{
			return m_Ib;
		}

		VertexBuffer GetVertexBuffer()
		{
			return m_Vb;
		}

	};

	Model LoadFromOBJ(const std::string& path, VertexArray& va);
	
	/*
	TODO:	1-  Separate parsing from loading in the GPU
			2-  Allow materials maybe add a ModelData struct (just for loading)
			3-  In the future, make more loaders for different formats
	*/

}