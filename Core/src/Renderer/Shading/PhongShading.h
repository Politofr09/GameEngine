#pragma once

#include "Shading.h"

namespace Core::Gfx
{
	// Very basic Blinn-phong shading implementation
	class PhongShading : public Shading
	{
	public:
		bool Load() override;
		void Apply(const Material& material) override;
	};

}
