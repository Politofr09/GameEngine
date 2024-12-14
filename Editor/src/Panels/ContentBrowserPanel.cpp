#include "ContentBrowserPanel.h"
#include "Core/Application.h"
#include <imgui/imgui.h>
#include <imgui/IconsFontAwesome5.h>

using namespace Core;
using namespace Gfx;

static std::filesystem::path normalizePath(std::filesystem::path& messyPath) 
{
    std::filesystem::path canonicalPath = std::filesystem::weakly_canonical(messyPath);
    std::string npath = canonicalPath.make_preferred().string();
    return npath;
}

ContentBrowserPanel::ContentBrowserPanel()
{
    m_FileIcon = Texture::Create("assets/icons/ContentBrowser@File.png");
}

void ContentBrowserPanel::OnImGuiRender()
{
    auto& registry = Application::Get()->GetCurrentProject().GetRegistry();

	ImGui::Begin("Content browser");

    // Sidebar
    ImGui::SetNextWindowBgAlpha(0.2f);
    ImGui::GetStyle().ChildRounding = 0.0f;
    ImGui::BeginChild("Sidebar", ImVec2(0.0f, 0.0f), ImGuiChildFlags_ResizeX);
    {
        m_SidebarWidth = ImGui::GetWindowSize().x;

        if (ImGui::TreeNodeEx(registry.GetAssetDirectory().c_str()))
        {
            if (ImGui::Button("Textures"))  m_SelectedAssetFilter = "Textures";
            if (ImGui::Button("Models"))    m_SelectedAssetFilter = "Models";
            if (ImGui::Button("Shaders"))   m_SelectedAssetFilter = "Shaders";
            if (ImGui::Button("Materials")) m_SelectedAssetFilter = "Materials";
            if (ImGui::Button("Fonts"))     m_SelectedAssetFilter = "Fonts";
            
            ImGui::TreePop();
        }
    }
    ImGui::EndChild();
    ImGui::GetStyle().ChildRounding = 5.0f;

    // Shadow effect
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    ImVec2 windowPos = ImGui::GetWindowPos();
    ImVec2 windowSize = ImGui::GetWindowSize();

    ImVec2 shadowStart = ImVec2(windowPos.x + m_SidebarWidth, 0.0f);
    ImVec2 shadowEnd = ImVec2(shadowStart.x - 20.0f, windowPos.y + windowSize.y);

    ImU32 shadowColorStart = IM_COL32(0, 0, 0, 100);
    ImU32 shadowColorEnd = IM_COL32(0, 0, 0, 0);

    // Draw gradient rectangle
    drawList->AddRectFilledMultiColor(
        shadowStart, shadowEnd,
        shadowColorStart, shadowColorEnd, shadowColorEnd, shadowColorStart
    );

    ImGui::SameLine();
    ImGui::BeginChild("MainContent", ImVec2(0, 0), true);
    {
        auto ShowAssetStorage = [&](const auto& storage, const std::string& assetType)
        {
            ImGui::SetCursorPosX((ImGui::GetWindowWidth() - ImGui::CalcTextSize(assetType.c_str()).x) * 0.5f);
            ImGui::Text(assetType.c_str());
            ImGui::Separator();

            float padding = 8.0f;
            float thumbnailSize = 96.0f;
            float cellSize = thumbnailSize + padding;
            float panelWidth = ImGui::GetWindowSize().x;
            int columnCount = (int)(panelWidth / cellSize);  // Calculate number of columns
            if (columnCount < 1) columnCount = 1;

            ImGui::Columns(columnCount, nullptr, false);  // Set up columns for the grid

            for (auto& pair : storage)
            {
                AssetMetadata metadata = pair.first;

                ImGui::PushID(metadata.ID);

                ImGui::SetNextWindowBgAlpha(0.2f);
                float columnWidth = ImGui::GetColumnWidth();  // Get the current column width
                ImGui::BeginChild(("Asset_" + std::to_string(metadata.ID)).c_str(), ImVec2(columnWidth - padding, 120), true);

                ImGui::Image((void*)(intptr_t)m_FileIcon->GetID(), ImVec2(thumbnailSize, thumbnailSize));
                ImGui::TextWrapped(metadata.Name.c_str());

                ImGui::EndChild();
                ImGui::NextColumn();
                ImGui::PopID();
            }
            ImGui::Columns(1); // Reset columns
        };


        if      (m_SelectedAssetFilter == "Textures") ShowAssetStorage(registry.GetTextureStorage(), "Textures");
        else if (m_SelectedAssetFilter == "Models") ShowAssetStorage(registry.GetTextureStorage(), "Models");
        else if (m_SelectedAssetFilter == "Shaders") ShowAssetStorage(registry.GetTextureStorage(), "Shaders");
        else if (m_SelectedAssetFilter == "Materials") ShowAssetStorage(registry.GetTextureStorage(), "Materials");
        else if (m_SelectedAssetFilter == "Fonts") ShowAssetStorage(registry.GetTextureStorage(), "Fonts");
    }
    ImGui::EndChild();

    ImGui::End();
}

