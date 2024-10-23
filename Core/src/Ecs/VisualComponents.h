#include "AssetRegistry.h"

#pragma once

namespace Core::Ecs
{

	struct ModelComponent
	{
		AssetHandle ModelHandle;
	};

	struct TextureComponent
	{
		AssetHandle TextureHandle;
	};

}