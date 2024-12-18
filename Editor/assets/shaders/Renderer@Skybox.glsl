#shader vertex
#version 330 core

out vec3 vTexCoord;

uniform mat4 uView;
uniform mat4 uProjection;

void main()
{
    vec3 Positions[8] = vec3[8](
        vec3(-1.0,  1.0, -1.0),  // Top-left near
        vec3(-1.0, -1.0, -1.0),  // Bottom-left near
        vec3( 1.0, -1.0, -1.0),  // Bottom-right near
        vec3( 1.0,  1.0, -1.0),  // Top-right near
        vec3(-1.0,  1.0,  1.0),  // Top-left far
        vec3(-1.0, -1.0,  1.0),  // Bottom-left far
        vec3( 1.0, -1.0,  1.0),  // Bottom-right far
        vec3( 1.0,  1.0,  1.0)   // Top-right far
    );

    int Indices[36] = int[36](
        0, 1, 2, 2, 3, 0, // Near face
        4, 5, 6, 6, 7, 4, // Far face
        4, 5, 1, 1, 0, 4, // Left face
        3, 2, 6, 6, 7, 3, // Right face
        0, 3, 7, 7, 4, 0, // Top face
        1, 2, 6, 6, 5, 1  // Bottom face
    );

    int i = Indices[gl_VertexID];
    vec3 position = Positions[i];

    // Pass through the direction for the cubemap
    vTexCoord = (vec4(position, 0.0) * mat4(mat3(uView))).xyz;

    // Transform to clip space
    gl_Position = uProjection * vec4(position, 1.0);
}

#shader fragment
#version 330 core

in vec3 vTexCoord;
out vec4 FragColor;

uniform samplerCube uCubemap;

void main()
{
    FragColor = texture(uCubemap, vTexCoord);
}
