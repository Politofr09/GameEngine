#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec3 vertexColor;
out vec2 uv;

uniform mat4 transform;

void main()
{
    vertexColor = aColor;
    uv = aTexCoord;
    gl_Position = transform * vec4(aPos, 1.0);
}
