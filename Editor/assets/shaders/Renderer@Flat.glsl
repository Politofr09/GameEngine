#shader vertex
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

#shader fragment
#version 330 core

out vec4 FragColor;
in vec3 vNormal;
in vec2 uv;

uniform sampler2D material_diffuse_texture;
uniform vec3 uMaterialColor;

void main()
{
    FragColor = texture(material_diffuse_texture, uv) * vec4(uMaterialColor, 1.0);
}