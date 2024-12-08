#shader vertex
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

    gl_Position = u_Projection * u_View * vec4(a_Position, 1.0f);
}

#shader fragment
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