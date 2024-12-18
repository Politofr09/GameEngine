#shader vertex
#version 330 core

out vec3 vWorldPos;

uniform mat4 uView;
uniform mat4 uProjection;
uniform vec3 uCameraWorldPos;
uniform float uGridSize = 100.0f;

const float GRID_LIMIT = 100.0;

const vec3 Positions[4] = vec3[4](
    vec3(-1.0, 0.0, -1.0),  // Bottom left
    vec3( 1.0, 0.0, -1.0),  // Bottom right
    vec3( 1.0, 0.0,  1.0),  // Top right
    vec3(-1.0, 0.0,  1.0)   // Top left
);
const int Indices[6] = int[6](0, 2, 1, 2, 0, 3);

void main()
{
    int i = Indices[gl_VertexID];
    vec4 position = vec4(Positions[i] * uGridSize, 1.0f);

    position.x += uCameraWorldPos.x;
    position.z += uCameraWorldPos.z;

    gl_Position = uProjection * uView * position;

    vWorldPos = position.xyz;
}

#shader fragment
#version 330 core

in vec3 vWorldPos;
out vec4 FragColor;

uniform float uGridCellSize = 0.8;
uniform vec4 uGridColorThin = vec4(0.5, 0.5, 0.5, 1.0);
uniform vec4 uGridColorThick = vec4(0.0, 0.0, 0.0, 1.0);
uniform vec3 uCameraWorldPos;
uniform float uGridSize = 100.0;
uniform float uGridMinPixelsBetweenCells = 2.0;

float log10(float x)
{
    float f = log(x) / log(10.0);
    return f;
}

float satf(float x)
{
    float f = clamp(x, 0.0, 1.0);
    return f;
}

vec2 satv(vec2 x)
{
    vec2 v = clamp(x, vec2(0.0), vec2(1.0));
    return v;
}

float max2(vec2 v)
{
    float f = max(v.x, v.y);
    return f;
}

void main()
{
    vec2 dvx = vec2(dFdx(vWorldPos.x), dFdy(vWorldPos.x));
    vec2 dvy = vec2(dFdx(vWorldPos.z), dFdy(vWorldPos.z));

    float lx = length(dvx);
    float ly = length(dvy);

    vec2 dudv = vec2(lx, ly);

    float l = length(dudv);

    float LOD = max(0.0, log10(l * uGridMinPixelsBetweenCells / uGridCellSize) + 1.0);

    float GridCellSizeLod0 = uGridCellSize * pow(10.0, floor(LOD));
    float GridCellSizeLod1 = GridCellSizeLod0 * 10.0;
    float GridCellSizeLod2 = GridCellSizeLod1 * 10.0;

    dudv *= 4.0;

    vec2 mod_div_dudv = mod(vWorldPos.xz, GridCellSizeLod0) / dudv;
    float Lod0a = max2(vec2(1.0) - abs(satv(mod_div_dudv) * 2.0 - vec2(1.0)) );

    mod_div_dudv = mod(vWorldPos.xz, GridCellSizeLod1) / dudv;
    float Lod1a = max2(vec2(1.0) - abs(satv(mod_div_dudv) * 2.0 - vec2(1.0)) );
    
    mod_div_dudv = mod(vWorldPos.xz, GridCellSizeLod2) / dudv;
    float Lod2a = max2(vec2(1.0) - abs(satv(mod_div_dudv) * 2.0 - vec2(1.0)) );

    float LOD_fade = fract(LOD);
    vec4 Color;

    if (Lod2a > 0.0) 
    {
        Color = uGridColorThick;
        Color.a *= Lod2a;
    } else 
    {
        if (Lod1a > 0.0) 
        {
            Color = mix(uGridColorThick, uGridColorThin, LOD_fade);
	        Color.a *= Lod1a;
        } else 
        {
            Color = uGridColorThin;
	        Color.a *= (Lod0a * (1.0 - LOD_fade));
        }
    }
    
    float OpacityFalloff = (1.0 - satf(length(vWorldPos.xz - uCameraWorldPos.xz) / uGridSize));

    Color.a *= OpacityFalloff;

    FragColor = Color;
}