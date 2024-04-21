#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out vec3 vNormal;
out vec2 uv;

uniform mat4 uTransform;
uniform mat4 uView;
uniform mat4 uProjection;

void main()
{
    vNormal = aNormal;
    uv = aTexCoord;
    gl_Position = uProjection * uView * uTransform * vec4(aPos, 1.0);
}
