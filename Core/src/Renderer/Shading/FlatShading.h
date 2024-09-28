#pragma once

#include "Shading.h"

namespace Core::Gfx
{

	class FlatShading : public Shading
	{
	public:
		bool Load() override;
		void Apply(const Material& material) override;
	};

}