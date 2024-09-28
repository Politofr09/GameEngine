#pragma once

#include "Shader.h"
#include "Material.h"

namespace Core::Gfx
{

	class Shading
	{
	public:
		virtual ~Shading() = default;

		virtual bool Load() = 0;

		// Bind shader and set all necessary uniforms from material
		virtual void Apply(const Material& material) = 0;

		Shader* ShaderProgram;
	};

}