#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

#pragma once

bool IsMouseHoveringTitleBar();

void DisplayRoundedImage(ImTextureID texture_id, ImVec2 size, float rounding, const ImVec2& uv0, const ImVec2& uv1, ImVec4 color = {1.0f, 1.0f, 1.0f, 1.0f});

