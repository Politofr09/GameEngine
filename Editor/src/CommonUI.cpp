#include "CommonUI.h"

bool IsMouseHoveringTitleBar()
{
    const float titleBarHeight = ImGui::GetFrameHeight();
    ImRect titleBarRect = ImRect(ImGui::GetWindowPos(), ImVec2(ImGui::GetWindowPos().x + ImGui::GetWindowSize().x, ImGui::GetWindowPos().y + titleBarHeight));
    return ImGui::IsMouseHoveringRect(titleBarRect.Min, titleBarRect.Max);
}

void DisplayRoundedImage(ImTextureID texture_id, ImVec2 size, float rounding, const ImVec2& uv0, const ImVec2& uv1, ImVec4 color)
{
    // Get the current window draw list
    ImDrawList* draw_list = ImGui::GetWindowDrawList();

    // Get the current cursor position
    ImVec2 pos = ImGui::GetCursorScreenPos();

    // Define the rectangle area for the image
    ImVec2 min = pos;
    ImVec2 max = ImVec2(pos.x + size.x, pos.y + size.y);

    // Draw the image
    ImGui::Image(texture_id, size, uv0, uv1, color);

    // Draw the rounded rectangle on top of the image
    draw_list->AddRect(min, max, ImGui::ColorConvertFloat4ToU32(ImGui::GetStyleColorVec4(ImGuiCol_WindowBg)), rounding, ImDrawFlags_RoundCornersAll, rounding);
}
