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
    m_CurrentDirectory = Application::Get()->GetCurrentProject().GetAssetDirectory();
    m_AssetDirectory = normalizePath(std::filesystem::path(Application::Get()->GetCurrentProject().GetAssetDirectory()));

    m_FolderIcon = Texture::Create("assets/icons/ContentBrowser@Folder.png");
    m_FileIcon = Texture::Create("assets/icons/ContentBrowser@File.png");
}

void ContentBrowserPanel::OnImGuiRender()
{
	ImGui::Begin("Content browser");

    // Sidebar
    ImGui::SetNextWindowBgAlpha(0.2f);
    ImGui::GetStyle().ChildRounding = 0.0f;
    ImGui::BeginChild("Sidebar", ImVec2(0.0f, 0.0f), ImGuiChildFlags_ResizeX);
    {
        m_SidebarWidth = ImGui::GetWindowSize().x;

        std::function<void(std::filesystem::path)> ShowDirectory = [&](std::filesystem::path path)
        {
            if (!std::filesystem::is_directory(path))
                return;

            std::string label = ICON_FA_FOLDER + std::string(" ") + path.filename().string();

            bool open = ImGui::TreeNodeEx(label.c_str(), ImGuiTreeNodeFlags_OpenOnDoubleClick);
            if (ImGui::IsItemClicked())
                m_CurrentDirectory = path;
            if (open)
            {

                for (const auto& entry : std::filesystem::directory_iterator(path))
                {
                    if (entry.is_directory())
                    {
                        // Recursively display subdirectories
                        ShowDirectory(entry.path());
                    }
                }

                ImGui::TreePop();
            }
        };
        ShowDirectory(m_AssetDirectory);
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
        float padding = 8.0f;
        float thumbnailSize = 96.0f;
        float cellSize = thumbnailSize + padding;
        float panelWidth = ImGui::GetWindowSize().x;
        int columnCount = (int)(panelWidth / cellSize);  // Calculate number of columns
        if (columnCount < 1) columnCount = 1;

        ImGui::Columns(columnCount, nullptr, false);  // Set up columns for the grid

        // Iterate over the directory contents
        for (const auto& entry : std::filesystem::directory_iterator(m_CurrentDirectory))
        {
            ImGui::PushID(entry.path().string().c_str());  // Unique ID for each item
            if (entry.is_directory())
            {
                ImGui::Image((void*)(intptr_t)m_FolderIcon->GetID(), ImVec2(thumbnailSize, thumbnailSize));
                if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0))
                {
                    m_CurrentDirectory = entry.path();
                }

                // Draw folder name
                ImGui::TextWrapped(entry.path().filename().string().c_str());

                ImGui::NextColumn();
            }
            else
            {
                ImGui::Image((void*)(intptr_t)m_FileIcon->GetID(), ImVec2(thumbnailSize, thumbnailSize));

                // Get the file extension
                std::string extension = entry.path().extension().string();
                if (!extension.empty() && extension[0] == '.')
                    extension.erase(0, 1); // Remove the leading dot

                // Calculate overlay position
                ImVec2 iconPos = ImGui::GetItemRectMin();
                ImVec2 iconSize = ImGui::GetItemRectSize();
                ImVec2 textPos = ImVec2(iconPos.x + iconSize.x / 4, iconPos.y + iconSize.y - 45);

                // Overlay the extension text
                ImGui::GetWindowDrawList()->AddText(
                    ImGui::GetFont(),                
                    ImGui::GetFontSize() + 10,       
                    textPos,
                    IM_COL32(0, 0, 0, 255),
                    extension.c_str(),
                    nullptr,
                    0.0f,
                    nullptr
                );
            }
            ImGui::PopID();
        }

        ImGui::Columns(1);  // Reset columns
    }
    ImGui::EndChild();

    ImGui::End();
}

void ContentBrowserPanel::ImportMenu()
{

}
