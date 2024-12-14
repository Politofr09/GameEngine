#pragma once

#include "Core/Asset.h"
#include "Renderer/Texture.h"
#include <filesystem>

class ContentBrowserPanel
{
public:
	ContentBrowserPanel();

	void OnImGuiRender();

private:
	std::string m_SelectedAssetFilter;
	float m_SidebarWidth = 200.0f;
	Core::Ref<Core::Gfx::Texture> m_FileIcon;
};