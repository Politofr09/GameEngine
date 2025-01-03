#include "ImGuiLayer.h"
#include "Core/Application.h"
#include "Instrumentor.h"

#include <iostream>

#include <imgui/imgui_internal.h>
#include <imgui/IconsFontAwesome5.h>
#include <imgui/ImGuizmo.h>
#include <GLFW/glfw3.h>

namespace Core
{
    // Define the static symbol "m_Dark" so the linker doesn't shout at my face
    bool ImGuiLayer::m_Dark = true;
    bool ImGuiLayer::m_BlockEvents = false;

    void SetupImGuiStyle(bool bStyleDark_, float alpha)
    {
        ImGuiStyle& style = ImGui::GetStyle();

        // Colors
        ImVec4* colors = style.Colors;

        if (bStyleDark_)
        {
            style.Colors[ImGuiCol_Text] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
            style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.4980392158031464f, 0.4980392158031464f, 0.4980392158031464f, 1.0f);
            style.Colors[ImGuiCol_WindowBg] = ImVec4(0.1137254908680916f, 0.1137254908680916f, 0.1137254908680916f, 1.0f);
            style.Colors[ImGuiCol_ChildBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
            style.Colors[ImGuiCol_PopupBg] = ImVec4(0.0784313753247261f, 0.0784313753247261f, 0.0784313753247261f, 0.9399999976158142f);
            style.Colors[ImGuiCol_Border] = ImVec4(1.0f, 1.0f, 1.0f, 0.1630901098251343f);
            style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
            style.Colors[ImGuiCol_FrameBg] = ImVec4(0.08627451211214066f, 0.08627451211214066f, 0.08627451211214066f, 1.0f);
            style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.1529411822557449f, 0.1529411822557449f, 0.1529411822557449f, 1.0f);
            style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.1882352977991104f, 0.1882352977991104f, 0.1882352977991104f, 1.0f);
            style.Colors[ImGuiCol_TitleBg] = ImVec4(0.1137254908680916f, 0.1137254908680916f, 0.1137254908680916f, 1.0f);
            style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.105882354080677f, 0.105882354080677f, 0.105882354080677f, 1.0f);
            style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.0f, 0.0f, 0.0f, 0.5099999904632568f);
            style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.1137254908680916f, 0.1137254908680916f, 0.1137254908680916f, 1.0f);
            style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.01960784383118153f, 0.01960784383118153f, 0.01960784383118153f, 0.5299999713897705f);
            style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.3098039329051971f, 0.3098039329051971f, 0.3098039329051971f, 1.0f);
            style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.407843142747879f, 0.407843142747879f, 0.407843142747879f, 1.0f);
            style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.5098039507865906f, 0.5098039507865906f, 0.5098039507865906f, 1.0f);
            style.Colors[ImGuiCol_CheckMark] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
            style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.8784313797950745f, 0.8784313797950745f, 0.8784313797950745f, 1.0f);
            style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.9803921580314636f, 0.9803921580314636f, 0.9803921580314636f, 1.0f);
            style.Colors[ImGuiCol_Button] = ImVec4(0.1490196138620377f, 0.1490196138620377f, 0.1490196138620377f, 1.0f);
            style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.2470588237047195f, 0.2470588237047195f, 0.2470588237047195f, 1.0f);
            style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.3294117748737335f, 0.3294117748737335f, 0.3294117748737335f, 1.0f);
            style.Colors[ImGuiCol_Header] = ImVec4(0.9764705896377563f, 0.9764705896377563f, 0.9764705896377563f, 0.3098039329051971f);
            style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.9764705896377563f, 0.9764705896377563f, 0.9764705896377563f, 0.800000011920929f);
            style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.9764705896377563f, 0.9764705896377563f, 0.9764705896377563f, 1.0f);
            style.Colors[ImGuiCol_Separator] = ImVec4(0.4274509847164154f, 0.4274509847164154f, 0.4980392158031464f, 0.5f);
            style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.7490196228027344f, 0.7490196228027344f, 0.7490196228027344f, 0.7803921699523926f);
            style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.7490196228027344f, 0.7490196228027344f, 0.7490196228027344f, 1.0f);
            style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.9764705896377563f, 0.9764705896377563f, 0.9764705896377563f, 0.2000000029802322f);
            style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.9372549057006836f, 0.9372549057006836f, 0.9372549057006836f, 0.6705882549285889f);
            style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.9764705896377563f, 0.9764705896377563f, 0.9764705896377563f, 0.9490196108818054f);
            style.Colors[ImGuiCol_Tab] = ImVec4(0.2235294133424759f, 0.2235294133424759f, 0.2235294133424759f, 0.8627451062202454f);
            style.Colors[ImGuiCol_TabHovered] = ImVec4(0.321568638086319f, 0.321568638086319f, 0.321568638086319f, 0.800000011920929f);
            style.Colors[ImGuiCol_TabActive] = ImVec4(0.2745098173618317f, 0.2745098173618317f, 0.2745098173618317f, 1.0f);
            style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.1450980454683304f, 0.1450980454683304f, 0.1450980454683304f, 0.9725490212440491f);
            style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.4235294163227081f, 0.4235294163227081f, 0.4235294163227081f, 1.0f);
            style.Colors[ImGuiCol_PlotLines] = ImVec4(0.6078431606292725f, 0.6078431606292725f, 0.6078431606292725f, 1.0f);
            style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.0f, 0.4274509847164154f, 0.3490196168422699f, 1.0f);
            style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.8980392217636108f, 0.6980392336845398f, 0.0f, 1.0f);
            style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.0f, 0.6000000238418579f, 0.0f, 1.0f);
            style.Colors[ImGuiCol_TableHeaderBg] = ImVec4(0.1882352977991104f, 0.1882352977991104f, 0.2000000029802322f, 1.0f);
            style.Colors[ImGuiCol_TableBorderStrong] = ImVec4(0.3098039329051971f, 0.3098039329051971f, 0.3490196168422699f, 1.0f);
            style.Colors[ImGuiCol_TableBorderLight] = ImVec4(0.2274509817361832f, 0.2274509817361832f, 0.2470588237047195f, 1.0f);
            style.Colors[ImGuiCol_TableRowBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
            style.Colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.0f, 1.0f, 1.0f, 0.05999999865889549f);
            style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.2588235437870026f, 0.5882353186607361f, 0.9764705896377563f, 0.3499999940395355f);
            style.Colors[ImGuiCol_DragDropTarget] = ImVec4(1.0f, 1.0f, 0.0f, 0.8999999761581421f);
            style.Colors[ImGuiCol_NavHighlight] = ImVec4(0.2588235437870026f, 0.5882353186607361f, 0.9764705896377563f, 1.0f);
            style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.0f, 1.0f, 1.0f, 0.699999988079071f);
            style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.800000011920929f, 0.800000011920929f, 0.800000011920929f, 0.2000000029802322f);
            style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.800000011920929f, 0.800000011920929f, 0.800000011920929f, 0.3499999940395355f);
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
            colors[ImGuiCol_TableHeaderBg] = ImVec4(0.95f, 0.95f, 0.98f, 1.00f);
            colors[ImGuiCol_TableBorderStrong] = ImVec4(0.30f, 0.30f, 0.45f, 1.00f);
            colors[ImGuiCol_TableBorderLight] = ImVec4(0.55f, 0.55f, 0.68f, 0.75f);
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

        style.Alpha = 1.0f;
        style.DisabledAlpha = 0.1000000014901161f;
        style.WindowPadding = ImVec2(8.0f, 8.0f);
        style.WindowRounding = 10.0f;
        style.WindowBorderSize = 0.0f;
        style.WindowMinSize = ImVec2(30.0f, 30.0f);
        style.WindowTitleAlign = ImVec2(0.5f, 0.5f);
        style.WindowMenuButtonPosition = ImGuiDir_Right;
        style.ChildRounding = 5.0f;
        style.ChildBorderSize = 0.0f;
        style.PopupRounding = 0.0f;
        style.PopupBorderSize = 1.0f;
        style.FramePadding = ImVec2(6.0f, 4.0f);
        style.FrameRounding = 5.0f;
        style.FrameBorderSize = 0.0f;
        style.ItemSpacing = ImVec2(5.0f, 4.0f);
        style.ItemInnerSpacing = ImVec2(5.0f, 5.0f);
        style.CellPadding = ImVec2(4.0f, 2.0f);
        style.IndentSpacing = 5.0f;
        style.ColumnsMinSpacing = 5.0f;
        style.ScrollbarSize = 15.0f;
        style.ScrollbarRounding = 9.0f;
        style.GrabMinSize = 15.0f;
        style.GrabRounding = 5.0f;
        style.TabRounding = 5.0f;
        style.TabBorderSize = 0.0f;
        style.TabMinWidthForCloseButton = 0.0f;
        style.ColorButtonPosition = ImGuiDir_Right;
        style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
        style.SelectableTextAlign = ImVec2(0.0f, 0.0f);
    }

    void ImGuiLayer::SetupImGui(bool dark)
    {
        CORE_PROFILE_FUNCTION();

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        // Setup styling and other ImGui settings
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable | ImGuiConfigFlags_ViewportsEnable;

        // Setup Platform/Renderer backends
        ImGui_ImplGlfw_InitForOpenGL(Core::Application::Get()->GetWindow()->GetHandle(), true);
        ImGui_ImplOpenGL3_Init("#version 330");

        //io.Fonts->AddFontDefault();
        float baseFontSize = 21.5f;
        io.Fonts->AddFontFromFileTTF("assets/fonts/sf-pro-text-medium.ttf", baseFontSize);

        float iconSize = baseFontSize * 2 / 3;
    
        // merge in icons from Font Awesome
        static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_16_FA, 0 };
        ImFontConfig icons_config;
        icons_config.MergeMode = true;
        icons_config.PixelSnapH = true;
        icons_config.GlyphMinAdvanceX = iconSize;
        io.Fonts->AddFontFromFileTTF(FONT_ICON_FILE_NAME_FAS, iconSize, &icons_config, icons_ranges);

        SetupImGuiStyle(dark, 1.0f);
        m_Dark = dark;
    }

    void ImGuiLayer::OnAttach()
    {
    }

    void ImGuiLayer::OnUpdate()
    {
    }

    void ImGuiLayer::OnImGuiRender()
    {
    }

    void ImGuiLayer::OnEvent(Core::Events::Event* event)
    {
        if (m_BlockEvents)
        {
            // TODO: Add event categories, in this case "EventCatagoryKeyboard" and "EventCatagoryMouse"
            ImGuiIO& io = ImGui::GetIO();
            if (event->GetType() == "KeyPressedEvent") event->Handled = io.WantCaptureKeyboard;
            if (event->GetType() == "KeyDownEvent") event->Handled = io.WantCaptureKeyboard;
            if (event->GetType() == "KeyReleasedEvent") event->Handled = io.WantCaptureKeyboard;

            if (event->GetType() == "MouseMovedEvent") event->Handled = io.WantCaptureMouse;
            if (event->GetType() == "MouseButtonPressedEvent") event->Handled = io.WantCaptureMouse;
            if (event->GetType() == "MouseScrollEvent") event->Handled = io.WantCaptureMouse;
        }
    }

    void ImGuiLayer::OnDettach()
    {
    }

    void ImGuiLayer::SwitchTheme(bool dark)
    {
        m_Dark = dark;
        SetupImGuiStyle(dark, 1.0f);
    }

    // Start new ImGui frame
    void ImGuiLayer::BeginImGuiContent()
    {
        CORE_PROFILE_FUNCTION();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGuizmo::BeginFrame();

        // Create a full-screen dockspace window
        ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDocking
            | ImGuiWindowFlags_NoTitleBar
            | ImGuiWindowFlags_MenuBar
            | ImGuiWindowFlags_NoCollapse
            | ImGuiWindowFlags_NoResize
            | ImGuiWindowFlags_NoMove
            | ImGuiWindowFlags_NoBringToFrontOnFocus
            | ImGuiWindowFlags_NoNavFocus;

        // Set up dockspace to be full-screen
        if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->Pos);
            ImGui::SetNextWindowSize(viewport->Size);
            ImGui::SetNextWindowViewport(viewport->ID);
        }
        else
        {
            ImGui::SetNextWindowPos(ImVec2(0, 0));
            ImGui::SetNextWindowSize(ImVec2(Core::Application::Get()->GetWindow()->GetWidth(), Core::Application::Get()->GetWindow()->GetHeight()));
        }

        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

        // Begin the dockspace window
        ImGui::Begin("Dockspace", NULL, windowFlags);

        // Pop the styles after the window begins
        ImGui::PopStyleVar(3);

        // Create the dockspace
        ImGuiID dockspaceID = ImGui::GetID("Dockspace");
        ImGui::DockSpace(dockspaceID, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);
    }


    // Render ImGui
    void ImGuiLayer::EndImGuiContent()
    {
        CORE_PROFILE_FUNCTION();
        // Dockspace end
        ImGui::End();

        ImGuiIO& io = ImGui::GetIO();

        ImGui::EndFrame();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    ImGuiLayer::~ImGuiLayer()
    {
        ImGui_ImplGlfw_Shutdown();
        ImGui_ImplOpenGL3_Shutdown();
    }

}