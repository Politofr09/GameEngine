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
uniform float uTime;

// float rand(vec2 co)
// {
//     return fract(sin(dot(co.xy, vec2(12.9898, 78.233))) * 43758.5453);
// }

// float noise(vec2 p)
// {
//     vec2 ip = floor(p);
//     vec2 u = fract(p);
//     u = u * u * (3.0 - 2.0 * u);

//     float res = mix(mix(rand(ip), rand(ip + vec2(1.0, 0.0)), u.x),
//                     mix(rand(ip + vec2(0.0, 1.0)), rand(ip + vec2(1.0, 1.0)), u.x), u.y);
//     return res * 2.0 - 1.0;
// }

void main()
{
    vNormal = aNormal;
    uv = aTexCoord;
    gl_Position = uProjection * uView * uTransform * vec4(aPos, 1.0);
    // gl_Position.x += noise(gl_Position.xy * 5.0 + uTime) * 0.1;
}

#shader fragment
#version 330 core

out vec4 FragColor;
in vec3 vNormal;
in vec2 uv;

uniform sampler2D ourTexture;

void main()
{
    FragColor = texture(ourTexture, uv);
}