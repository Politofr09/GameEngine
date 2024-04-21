#include <GL/glew.h>
#include "Core/Utils.h"

#include "VertexArray.h"
#include "IndexBuffer.h"

#pragma once

#define GLCall(x) Core::Gfx::GLClearError(); \
    x;\
    ASSERT(Core::Gfx::GLLogCall());

namespace Core::Gfx
{
    void GLClearError();
    bool GLLogCall();

    class Renderer
    {
    public:
        Renderer() = default;

        void Clear();
        void Draw(const VertexArray& va, const IndexBuffer& ib);
    };

}