#include "ImGuiLayer.h"
#include <imgui/imgui_internal.h>
#include <imgui/IconsFontAwesome5.h>
#include <GLFW/glfw3.h>
#include <iostream>

// Define the static symbol "m_Dark" so the linker doesn't shout at my face
bool Core::ImGuiLayer::m_Dark = true;
bool Core::ImGuiLayer::m_OnTop = false;

// Very large function holding the engine's dark and white themes
// Dark theme: 'Midnight Savanna' custom theme for this occasion
// White theme: 'Crystal Clear' also custom theme
// This themes go well with San Francisco font and Roboto font, I tested them with both.
void SetupImGuiStyle(bool bStyleDark_, float alpha)
{
    ImGuiStyle& style = ImGui::GetStyle();

    // Colors
    ImVec4* colors = style.Colors;

    if (bStyleDark_)
    {
        // Dark theme with orange accent
        colors[ImGuiCol_Text] = ImVec4(0.80f, 0.80f, 0.80f, alpha);
        colors[ImGuiCol_TextDisabled] = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);
        colors[ImGuiCol_WindowBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
        colors[ImGuiCol_ChildBg] = ImVec4(0.15f, 0.15f, 0.15f, 0.00f);
        colors[ImGuiCol_PopupBg] = ImVec4(0.20f, 0.20f, 0.20f, 0.98f);
        colors[ImGuiCol_Border] = ImVec4(0.30f, 0.30f, 0.30f, 0.30f);
        colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_FrameBg] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
        colors[ImGuiCol_FrameBgHovered] = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);
        colors[ImGuiCol_FrameBgActive] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
        colors[ImGuiCol_TitleBg] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
        colors[ImGuiCol_TitleBgActive] = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);
        colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
        colors[ImGuiCol_MenuBarBg] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
        colors[ImGuiCol_ScrollbarBg] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
        colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.82f, 0.47f, 0.22f, 1.00f);
        colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.92f, 0.57f, 0.32f, 1.00f);
        colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(1.00f, 0.67f, 0.40f, 1.00f);
        colors[ImGuiCol_CheckMark] = ImVec4(0.82f, 0.47f, 0.22f, 1.00f);
        colors[ImGuiCol_SliderGrab] = ImVec4(0.82f, 0.47f, 0.22f, 1.00f);
        colors[ImGuiCol_SliderGrabActive] = ImVec4(0.92f, 0.57f, 0.32f, 1.00f);
        colors[ImGuiCol_Button] = ImVec4(0.82f, 0.47f, 0.22f, 1.00f);
        colors[ImGuiCol_ButtonHovered] = ImVec4(0.92f, 0.57f, 0.32f, 1.00f);
        colors[ImGuiCol_ButtonActive] = ImVec4(0.72f, 0.37f, 0.12f, 1.00f);
        colors[ImGuiCol_Header] = ImVec4(0.20f, 0.20f, 0.20f, 0.53f);
        colors[ImGuiCol_HeaderHovered] = ImVec4(0.28f, 0.28f, 0.28f, 0.73f);
        colors[ImGuiCol_HeaderActive] = ImVec4(0.33f, 0.33f, 0.33f, 1.00f);
        colors[ImGuiCol_Separator] = ImVec4(0.50f, 0.50f, 0.50f, 0.50f);
        colors[ImGuiCol_SeparatorHovered] = ImVec4(0.90f, 0.90f, 0.90f, 0.78f);
        colors[ImGuiCol_SeparatorActive] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
        colors[ImGuiCol_ResizeGrip] = ImVec4(0.50f, 0.50f, 0.50f, 0.56f);
        colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.70f, 0.70f, 0.70f, 0.67f);
        colors[ImGuiCol_ResizeGripActive] = ImVec4(0.90f, 0.90f, 0.90f, 0.95f);
        colors[ImGuiCol_Tab] = ImVec4(0.22f, 0.22f, 0.22f, 0.86f);
        colors[ImGuiCol_TabHovered] = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);
        colors[ImGuiCol_TabActive] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
        colors[ImGuiCol_TabUnfocused] = ImVec4(0.20f, 0.20f, 0.20f, 0.56f);
        colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
        colors[ImGuiCol_TextSelectedBg] = ImVec4(0.28f, 0.28f, 0.28f, 0.35f);
        colors[ImGuiCol_DragDropTarget] = ImVec4(0.82f, 0.47f, 0.22f, 1.00f);
        colors[ImGuiCol_NavHighlight] = ImVec4(0.82f, 0.47f, 0.22f, 1.00f);
        colors[ImGuiCol_NavWindowingHighlight] = ImVec4(0.82f, 0.47f, 0.22f, 0.70f);
        colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.20f, 0.20f, 0.20f, 0.20f);
        colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);

    }
    else
    {
        // White with blue accent, 'Crystal Clear'
        colors[ImGuiCol_Text] = ImVec4(0.00f, 0.00f, 0.00f, alpha);
        colors[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
        colors[ImGuiCol_WindowBg] = ImVec4(0.94f, 0.94f, 0.96f, 1.00f);
        colors[ImGuiCol_ChildBg] = ImVec4(0.92f, 0.92f, 0.92f, 0.00f);
        colors[ImGuiCol_PopupBg] = ImVec4(1.00f, 1.00f, 1.00f, 0.98f);
        colors[ImGuiCol_Border] = ImVec4(0.00f, 0.00f, 0.00f, 0.30f);
        colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_FrameBg] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
        colors[ImGuiCol_FrameBgHovered] = ImVec4(0.90f, 0.90f, 0.90f, 1.00f);
        colors[ImGuiCol_FrameBgActive] = ImVec4(0.95f, 0.95f, 0.95f, 1.00f);
        colors[ImGuiCol_TitleBg] = ImVec4(0.78f, 0.78f, 0.80f, 1.00f);
        colors[ImGuiCol_TitleBgActive] = ImVec4(0.52f, 0.52f, 0.53f, 1.00f);
        colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.78f, 0.78f, 0.80f, 1.00f);
        colors[ImGuiCol_MenuBarBg] = ImVec4(0.86f, 0.86f, 0.88f, 1.00f);
        colors[ImGuiCol_ScrollbarBg] = ImVec4(0.98f, 0.98f, 0.98f, 0.53f);
        colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.69f, 0.69f, 0.69f, 1.00f);
        colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.49f, 0.49f, 0.49f, 1.00f);
        colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
        colors[ImGuiCol_CheckMark] = ImVec4(0.28f, 0.59f, 1.00f, 1.00f);
        colors[ImGuiCol_SliderGrab] = ImVec4(0.28f, 0.59f, 1.00f, 1.00f);
        colors[ImGuiCol_SliderGrabActive] = ImVec4(0.38f, 0.69f, 1.00f, 1.00f);
        colors[ImGuiCol_Button] = ImVec4(0.38f, 0.69f, 1.00f, 1.00f);
        colors[ImGuiCol_ButtonHovered] = ImVec4(0.28f, 0.59f, 1.00f, 1.00f);
        colors[ImGuiCol_ButtonActive] = ImVec4(0.22f, 0.49f, 0.87f, 1.00f);
        colors[ImGuiCol_Header] = ImVec4(0.38f, 0.69f, 1.00f, 0.53f);
        colors[ImGuiCol_HeaderHovered] = ImVec4(0.38f, 0.69f, 1.00f, 0.73f);
        colors[ImGuiCol_HeaderActive] = ImVec4(0.38f, 0.69f, 1.00f, 1.00f);
        colors[ImGuiCol_Separator] = ImVec4(0.39f, 0.39f, 0.39f, 0.50f);
        colors[ImGuiCol_SeparatorHovered] = ImVec4(0.14f, 0.44f, 0.80f, 0.78f);
        colors[ImGuiCol_SeparatorActive] = ImVec4(0.14f, 0.44f, 0.80f, 1.00f);
        colors[ImGuiCol_ResizeGrip] = ImVec4(0.80f, 0.80f, 0.80f, 0.56f);
        colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.87f, 0.87f, 0.87f, 0.67f);
        colors[ImGuiCol_ResizeGripActive] = ImVec4(0.87f, 0.87f, 0.87f, 0.95f);
        colors[ImGuiCol_Tab] = ImVec4(0.37f, 0.65f, 0.98f, 0.86f);
        colors[ImGuiCol_TabHovered] = ImVec4(0.28f, 0.59f, 1.00f, 1.00f);
        colors[ImGuiCol_TabActive] = ImVec4(0.39f, 0.67f, 1.00f, 1.00f);
        colors[ImGuiCol_TabUnfocused] = ImVec4(0.95f, 0.95f, 0.95f, 0.56f);
        colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.68f, 0.68f, 0.68f, 1.00f);
        colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
        colors[ImGuiCol_DragDropTarget] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
        colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
        colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
        colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
        colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);
    }
    // Style
    style.Alpha = 1.0f;
    style.WindowPadding = ImVec2(8, 8);
    style.WindowRounding = 7.0f;
    style.WindowBorderSize = 1.0f;
    style.WindowMinSize = ImVec2(32, 32);
    style.WindowTitleAlign = ImVec2(0.5f, 0.5f);
    style.ChildRounding = 0.0f;
    style.ChildBorderSize = 1.0f;
    style.PopupRounding = 3.0f;
    style.PopupBorderSize = 1.0f;
    style.FramePadding = ImVec2(4, 3);
    style.FrameRounding = 3.0f;
    style.FrameBorderSize = 1.0f;
    style.ItemSpacing = ImVec2(8, 4);
    style.ItemInnerSpacing = ImVec2(4, 4);
    style.TouchExtraPadding = ImVec2(0, 0);
    style.IndentSpacing = 21.0f;
    style.ColumnsMinSpacing = 6.0f;
    style.ScrollbarSize = 16.0f;
    style.ScrollbarRounding = 9.0f;
    style.GrabMinSize = 10.0f;
    style.GrabRounding = 3.0f;
    style.TabRounding = 3.0f;
    style.TabBorderSize = 1.0f;
    style.SelectableTextAlign = ImVec2(0.0f, 0.0f);
}

void Core::ImGuiLayer::SetupImGui(Window* window, bool dark, bool useRobotoFont)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    // Setup styling and other ImGui settings
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window->GetHandle(), true);
    ImGui_ImplOpenGL3_Init("#version 330");


    if (useRobotoFont)
    {
        io.Fonts->AddFontFromFileTTF("res/Roboto-Regular.ttf", 18.0f);
    } 
    else // -> use San Francisco font
    {
        //io.Fonts->AddFontDefault();
        float baseFontSize = 21.5f;
        float iconSize = baseFontSize * 2 / 3;
        io.Fonts->AddFontFromFileTTF("res/sf-pro-text-medium.ttf", baseFontSize);

        // merge in icons from Font Awesome
        static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_16_FA, 0 };
        ImFontConfig icons_config;
        icons_config.MergeMode = true;
        icons_config.PixelSnapH = true;
        icons_config.GlyphMinAdvanceX = iconSize;
        io.Fonts->AddFontFromFileTTF(FONT_ICON_FILE_NAME_FAS, iconSize, &icons_config, icons_ranges);
    }

    SetupImGuiStyle(dark, 1.0f);
    m_Dark = dark;
}

void Core::ImGuiLayer::OnAttach()
{
}

void Core::ImGuiLayer::OnUpdate()
{
}

void Core::ImGuiLayer::OnImGuiRender()
{
}

void Core::ImGuiLayer::OnEvent(Core::Events::Event* event)
{
}

void Core::ImGuiLayer::OnDettach()
{
}

void Core::ImGuiLayer::SwitchTheme(bool dark)
{
    m_Dark = dark;
    SetupImGuiStyle(dark, 1.0f);
}

// Start new ImGui frame
void Core::ImGuiLayer::BeginImGuiContent()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

// Render ImGui
void Core::ImGuiLayer::EndImGuiContent()
{
    ImGuiIO& io = ImGui::GetIO();
    m_OnTop = ImGui::IsWindowFocused(ImGuiFocusedFlags_AnyWindow);

    ImGui::EndFrame();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Core::ImGuiLayer::DisplayRoundedImage(ImTextureID texture_id, ImVec2 size, float rounding, const ImVec2& uv0, const ImVec2& uv1, ImVec4 color)
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

bool Core::ImGuiLayer::IsMouseHoveringTitleBar()
{
    const float titleBarHeight = ImGui::GetFrameHeight();
    ImRect titleBarRect = ImRect(ImGui::GetWindowPos(), ImVec2(ImGui::GetWindowPos().x + ImGui::GetWindowSize().x, ImGui::GetWindowPos().y + titleBarHeight));
    return ImGui::IsMouseHoveringRect(titleBarRect.Min, titleBarRect.Max);
}

Core::ImGuiLayer::~ImGuiLayer()
{
    ImGui_ImplGlfw_Shutdown();
    ImGui_ImplOpenGL3_Shutdown();
}