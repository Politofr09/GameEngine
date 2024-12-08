#pragma once

#include "Core/Asset.h"
#include "Renderer/Texture.h"
#include <filesystem>

class ContentBrowserPanel
{
public:
	ContentBrowserPanel();
	void OpenAssetDirectory(const std::string& path) { m_AssetDirectory = path; };

	void OnImGuiRender();

	void ImportMenu();

private:
	std::filesystem::path m_CurrentDirectory;
	std::filesystem::path m_AssetDirectory;
	Core::Ref<Core::Gfx::Texture> m_FolderIcon;
	Core::Ref<Core::Gfx::Texture> m_FileIcon;
	float m_SidebarWidth = 200.0f;
};