#shader vertex
#version 330 core

layout (location = 1) in vec3 a_Position;
layout (location = 1) in vec4 a_Color;

out vec4 v_Color;

uniform mat4 u_Projection;
uniform mat4 u_View;

void main()
{
    v_Color = a_Color;
    gl_Position = u_Projection * u_View * vec4(a_Position, 1.0f);
}

#shader fragment
#version 330 core

in vec4 v_Color;

out vec4 color;

void main()
{
    color = v_Color;    
}
