#shader vertex

#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out vec3 vNormal;
out vec3 vPos;
out vec2 uv;

uniform mat4 uTransform;
uniform mat4 uView;
uniform mat4 uProjection;

void main()
{
    vNormal = mat3(transpose(inverse(uTransform))) * aNormal;  // Adjust normal based on the model's transformation
    vPos = vec3(uTransform * vec4(aPos, 1.0));  // Transformed vertex position
    uv = aTexCoord;
    gl_Position = uProjection * uView * uTransform * vec4(aPos, 1.0);
}

#shader fragment
#version 330 core

out vec4 FragColor;
in vec3 vNormal;
in vec3 vPos;
in vec2 uv;

uniform sampler2D material_diffuse_texture;

// Phong model uniforms
uniform vec3 uLightPosition;
uniform vec3 uLightColor;
uniform vec3 uViewPosition;

uniform vec3 uMaterialColor;
uniform vec3 uMaterialAmbient;
uniform vec3 uMaterialDiffuse;
uniform vec3 uMaterialSpecular;
uniform float uMaterialShininess;

void main()
{
    // Ambient
    vec3 ambient = uMaterialAmbient * uLightColor;

    // Diffuse
    vec3 norm = normalize(vNormal);
    vec3 lightDir = normalize(uLightPosition - vPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * uMaterialDiffuse * uLightColor;

    // Specular (Blinn-Phong)
    vec3 viewDir = normalize(uViewPosition - vPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);  // Halfway vector
    float spec = pow(max(dot(norm, halfwayDir), 0.0), uMaterialShininess);
    vec3 specular = uMaterialSpecular * spec * uLightColor;

    // Combine results
    vec3 lighting = ambient + diffuse + specular;
    vec4 textureColor = texture(material_diffuse_texture, uv);
    FragColor = vec4(lighting, 1.0) * textureColor * vec4(uMaterialColor, 1.0);
}
