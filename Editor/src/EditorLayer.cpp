
#include "EditorLayer.h"
#include "Core/Keyboard.h"
#include "Core/Mouse.h"
#include "Core/Asset.h"
#include "Core/Application.h"
#include "Core/Instrumentor.h"
#include "Events/Event.h"
#include "Renderer/Renderer.h"
#include "Renderer/Renderer2D.h"
#include "Renderer/TextRenderer.h"
#include "Ecs/ECS.h"

#include "CommonUI.h"

#include <iostream>
#include <filesystem>

#include <imgui/imgui.h>
#include <imgui/ImGuizmo.h>
#include <imgui/IconsFontAwesome5.h>
#include <tinyfiledialogs/tinyfiledialogs.h>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace Core::Gfx;
using namespace Core;

EditorLayer::EditorLayer() 
    : Layer("EditorLayer"), m_SceneHierarchyPanel(Core::Application::Get()->GetCurrentProject().GetScene()) 
{}

void EditorLayer::OnAttach()
{
    CORE_PROFILE_FUNCTION();

    // Setup event
	Events::Dispatcher::Subscribe(std::bind(&EditorLayer::OnEvent, this, std::placeholders::_1));
	
    int width = Application::Get()->GetWindow()->GetWidth();
    int height = Application::Get()->GetWindow()->GetHeight();

    //// Setup *OpenGL* renderer
    Renderer::Init();
    Renderer2D::Init();
    TextRenderer::Init();

    /**** Setup the camera(s) ****/
    m_Cam.SetAspectRatio((float)width / height);
    m_Cam2d = Core::Gfx::OrthographicCamera(0.0f, (float)width, (float)height, 0.0f);
    
    m_Framebuffer.Create(width, height);
}

static int FONT_SCALE = 3;

void EditorLayer::OnUpdate()
{
    // UpdateCameraController();
    //shader->SetFloat("uTime", (float)glfwGetTime());
    Renderer::SetBackgroundColor({ 0.2f, 0.3f, 0.3f });
    Renderer::Clear();
    Light light;
    light.Position = { 0.0f, 10.0f, 0.0f };
    Renderer::SetLight(light);
    m_Framebuffer.Bind();
    Renderer::BeginScene(m_Cam);
    {
        Renderer::Clear();

        // Eventually this will be more complex, we will have a list of scenes that we can edit with the editor layer
        Renderer::RenderScene(Application::Get()->GetCurrentProject().GetScene());
    }
    Renderer::EndScene();

    // Testing Renderer2D
    Renderer2D::Begin(m_Cam2d);
    {
        auto& registry = Application::Get()->GetCurrentProject().GetScene().GetRegistry();
        auto& view = registry.GetView<Ecs::TransformComponent, Ecs::SpriteComponent>();
        for (auto e : view)
        {
            auto& t = registry.GetComponent<Ecs::TransformComponent>(e);
            auto& s = registry.GetComponent<Ecs::SpriteComponent>(e);

            if (s.TextureHandle)
            {
                Ref<Texture> texture = Application::Get()->GetCurrentProject().GetRegistry().Get<Texture>(s.TextureHandle);

                Renderer2D::DrawQuadTextured(glm::vec3(t.Position), glm::vec2(t.Scale.x, t.Scale.y), s.Color, texture);
            }
            else
            {
                Renderer2D::DrawQuad(glm::vec3(t.Position), glm::vec2(t.Scale.x, t.Scale.y), s.Color);
            }
        }

        // Manual test: Rendering lines
        //Renderer2D::DrawLine(glm::vec2(100.0f, 100.0f), glm::vec2(200.0f, 200.0f), glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));
    }
    Renderer2D::End();
    m_Framebuffer.UnBind();
}

void EditorLayer::OnImGuiRender()
{
    // ImGui content
    //ImGui::Begin("Menubar", (bool*)0, ImGuiWindowFlags_MenuBar);
    //CORE_PROFILE_FUNCTION();

    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Exit"))
            {
                Application::Get()->GetWindow()->Close();
            }
            if (ImGui::MenuItem("Open"))
            {
                const char* filterPatterns[1] = { "*.yaml" };
                const char* path = tinyfd_openFileDialog("Select a project file", "", 1, filterPatterns, NULL, 0);

                if (path)
                {
                    Application::Get()->LoadProject(path);
                }
            }
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("View"))
        {
            ImGui::MenuItem("Toggle Camera Controll", NULL, &showCameraControl);
            ImGui::MenuItem("Toggle Theme Switcher", NULL, &showThemeSwitcher);
            ImGui::MenuItem("Toggle Asset Registry", NULL, &showAssetRegistry);
            ImGui::MenuItem("Toggle Statistics", NULL, &showRenderingSettings);
            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }

    ImGui::ShowDemoWindow();

    if (showCameraControl)      ShowCameraControlImgui(&showCameraControl);
    if (showAssetRegistry)      ShowAssetRegistry(&showAssetRegistry);
    if (showThemeSwitcher)      ShowThemeSwitcher(&showThemeSwitcher);
    //if (showECSPanel)           ShowECSPanel(&showECSPanel);
    m_SceneHierarchyPanel.OnImGuiRender();
    m_ContentBrowserPanel.OnImGuiRender();

    if (showRenderingSettings)  ShowRenderingSettings(&showRenderingSettings);

    static ImVec2 availableSpace = ImVec2();
    ImGui::GetStyle().WindowPadding = { 1.5f, 1.5f };
    if (ImGui::Begin("VIEWPORT", 0, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse))
    {
        if (availableSpace.x != ImGui::GetContentRegionAvail().x || availableSpace.y != ImGui::GetContentRegionAvail().y)
        {
            availableSpace = ImGui::GetContentRegionAvail();
            m_Framebuffer.Resize((int)availableSpace.x, (int)availableSpace.y);
            m_Cam.OnViewportResize((int)availableSpace.x, (int)availableSpace.y);
            m_Cam2d.OnViewportResize((int)availableSpace.x, (int)availableSpace.y);
        }

        ImGui::BeginChild("Viewport", availableSpace, false, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoMove);
        //DisplayRoundedImage((ImTextureID)m_Framebuffer.GetTextureID(), ImVec2(m_Framebuffer.GetWidth(), m_Framebuffer.GetHeight()), 0.0f, ImVec2(0, 1), ImVec2(1, 0));

        Application::Get()->GetImGuiLayer()->BlockEvents(!ImGui::IsWindowHovered());
        ImGui::Image((ImTextureID)m_Framebuffer.GetTextureID(), ImVec2(m_Framebuffer.GetWidth(), m_Framebuffer.GetHeight()), ImVec2(0, 1), ImVec2(1, 0));

        // Gizmos
        auto& entity = m_SceneHierarchyPanel.GetSelectedEntity();
        if (entity.GetID())
        {
            ImGuizmo::SetOrthographic(false);
            ImGuizmo::SetDrawlist();

            glm::mat4 modelMatrix = entity.GetComponent<Ecs::TransformComponent>();

            ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, m_Framebuffer.GetWidth(), m_Framebuffer.GetHeight());

            bool snap = Input::Keyboard::IsKeyDown(GLFW_KEY_LEFT_CONTROL);
            float snapValue = 0.5f;
            if (m_GizmoOperation == ImGuizmo::OPERATION::ROTATE) 
                snapValue = 15.0f;

            float snapValues[3] = { snapValue, snapValue, snapValue };

            ImGuizmo::Manipulate(glm::value_ptr(m_Cam.GetViewMatrix()), glm::value_ptr(m_Cam.GetProjectionMatrix()),
                (ImGuizmo::OPERATION)m_GizmoOperation,
                ImGuizmo::LOCAL, 
                glm::value_ptr(modelMatrix),
                nullptr,
                snap ? snapValues : nullptr
            );

            if (ImGuizmo::IsUsing())
            {
                auto& transform = entity.GetComponent<Ecs::TransformComponent>();

                // Extract translation, rotation, and scale from the model matrix
                glm::vec3 translation, scale;
                glm::quat rotation;
                glm::vec3 skew;
                glm::vec4 perspective;

                bool success = glm::decompose(modelMatrix, scale, rotation, translation, skew, perspective);
                if (success)
                {
                    transform.Position = translation;
                    transform.Rotation = glm::eulerAngles(rotation);  // Convert quaternion to Euler angles
                    transform.Scale = scale;
                }
            }
        }

        // TODO: Rework event system and clean everything up
        // - Event categories
        // - Better API
        // - Event& instead of Event* pointer it's not comfortable enough
        // - dynamic_cast... bad
        // - EventDispatcher rework
        
        // TODO
        // ... so we can isolate key events and mouse inputs
        // like block the events for imgui,
        // or just let the camera move
        // but if imguizmo is active ignore camera key input...
        // ...

        if (ImGui::IsWindowHovered() && !ImGuizmo::IsUsing()) m_Cam.Move();

        ImGui::EndChild();
    }
    ImGui::End();
}

void EditorLayer::ShowCameraControlImgui(bool* p_open)
{
    if (ImGui::Begin("Switch perspective", p_open))
    {
        static float in_FOV = 75.0F;
        if (ImGui::SliderFloat("FOV", &in_FOV, 30.0f, 120.0f))
        {
            float aspectRatio = m_Framebuffer.GetAspectRatio();
            m_Cam.SetProjectionMatrix(glm::perspective(glm::radians(in_FOV), aspectRatio, 0.01f, 1000.0f));
            //shader->SetMatrix("uProjection", m_Cam.GetProjectionMatrix());
        }

        ImGui::SameLine();
        if (ImGui::Button("Reset"))
        {
            in_FOV = 75.0F;
            float aspectRatio = m_Framebuffer.GetAspectRatio();
            m_Cam.SetProjectionMatrix(glm::perspective(glm::radians(in_FOV), aspectRatio, 0.01f, 1000.0f));
            //shader->SetMatrix("uProjection", m_Cam.GetProjectionMatrix());
        }

        // Display a 2D representation of the perspective frustum
        ImVec2 startPos = ImGui::GetCursorScreenPos();
        startPos.x += ImGui::GetWindowSize().x / 2;
        startPos.y += 100;
        ImVec2 size = ImVec2(100.0f, 100.0f); // Adjust size as needed
        ImVec2 endPos = ImVec2(startPos.x + size.x, startPos.y + size.y);

        // Draw the outline of the frustum
        float halfFOV = glm::radians(in_FOV) / 2.0f;
        float nearDistance = 0.0f;
        float farDistance = 1000.0f;
        float aspectRatio = (float)Application::Get()->GetWindow()->GetWidth() / Application::Get()->GetWindow()->GetHeight();

        float nearHeight = tan(halfFOV) * nearDistance / 20.0f;
        float farHeight = tan(halfFOV) * farDistance / 20.0f;

        ImColor col1(ImGui::GetStyle().Colors[ImGuiCol_Text]);
        ImColor col2(ImGui::GetStyle().Colors[ImGuiCol_Button]);

        ImGui::GetWindowDrawList()->AddLine(ImVec2(startPos.x + size.x / 2, endPos.y - nearDistance * 25.0f), ImVec2(startPos.x - size.x / 2, endPos.y - nearDistance * 25.0f), col1, 2.0f);
        ImGui::GetWindowDrawList()->AddLine(ImVec2(startPos.x + size.x / 2, endPos.y - farDistance / 10.0f), ImVec2(startPos.x - size.x / 2, endPos.y - farDistance / 10.0f), col1, 2.0f);

        ImGui::GetWindowDrawList()->AddLine(ImVec2(startPos.x, endPos.y), ImVec2(startPos.x + farHeight / 2, startPos.y - 25), col2, 2.0f);
        ImGui::GetWindowDrawList()->AddLine(ImVec2(startPos.x, endPos.y), ImVec2(startPos.x - farHeight / 2, startPos.y - 25), col2, 2.0f);
    }
    ImGui::End();
}

void EditorLayer::ShowThemeSwitcher(bool* p_open)
{
    //if (!p_open) return;

    if (ImGui::Begin("Switch theme", p_open))
    {
        if (ImGui::Button("Switch to dark/light mode"))
        {
            ImGuiLayer::SwitchTheme(!ImGuiLayer::IsDarkThemed());
        }

        // ImGui::Text(ICON_FA_PAINT_BRUSH "  Paint"           );
        // ImGui::Text(ICON_FA_ARROW_DOWN  "  Down"            );
        // ImGui::Text(ICON_FA_GAMEPAD     "  Preview"         );
        // ImGui::Text(ICON_FA_RUNNING     "  Animation"       );
        // ImGui::Text(ICON_FA_CAMERA      "  Camera"          );
        // ImGui::Text(ICON_FA_BUG         "  Debug"           );
        // ImGui::Text(ICON_FA_TOILET_PAPER"  Toilet paper"    );
        // ImGui::Text(ICON_FA_CLOCK       "  Clock"           );
        // ImGui::Text(ICON_FA_CODE        "  Coding"          );
        // ImGui::Text(ICON_FA_COG         "  Settings"        );
        // ImGui::Text(ICON_FA_MUSIC       "  Music"           );
    }
    ImGui::End();
}

void EditorLayer::ShowAssetRegistry(bool* p_open)
{
    if (!p_open) return;

    if (ImGui::Begin("Asset Registry", p_open))
    {
        AssetRegistry& registry = Application::Get()->GetCurrentProject().GetRegistry();

        //size_t totalAssets = registry.GetTextureStorage().size() +
        //    registry.GetModelStorage().GetAllResources().size() +
        //    registry.GetMaterialStorage().GetAllResources().size() +
        //    registry.GetShaderStorage().GetAllResources().size() +
        //    registry.GetFontStorage().GetAllResources().size();

        //ImGui::Text("Number of assets: %zu", totalAssets);

        if (ImGui::BeginTable("Resource Table", 4, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg))
        {
            // Set up column headers
            ImGui::TableSetupColumn("Asset Name", ImGuiTableColumnFlags_WidthStretch);
            ImGui::TableSetupColumn("Asset Type", ImGuiTableColumnFlags_WidthStretch);
            ImGui::TableSetupColumn("Asset ID", ImGuiTableColumnFlags_WidthStretch);
            ImGui::TableSetupColumn("Preview", ImGuiTableColumnFlags_WidthFixed, 100.0f);
            ImGui::TableHeadersRow();

            // Helper lambda to display assets from a specific storage
            auto displayAssets = [&](const auto& storage, const std::string& assetType) {
                for (const auto& [metadata, asset] : storage)
                {
                    uint64_t assetID = metadata.ID;

                    ImGui::TableNextRow();

                    // Asset Name
                    ImGui::TableSetColumnIndex(0);
                    ImGui::Button(metadata.Name.c_str());

                    // Drag and drop asset IDs
                    if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID))
                    {
                        std::string item_name = "ASSET_DND_" + assetType;
                        ImGui::SetDragDropPayload(item_name.c_str(), &assetID, sizeof(assetID));

                        // Display the asset ID in the UI
                        ImGui::Text("%s: %llu", assetType.c_str(), assetID); // Use %llu for uint64_t

                        ImGui::EndDragDropSource();
                    }

                    // Asset Type
                    ImGui::TableSetColumnIndex(1);
                    ImGui::TextUnformatted(assetType.c_str());

                    // Asset ID
                    ImGui::TableSetColumnIndex(2);
                    ImGui::Text("%llu", assetID);

                    // Preview
                    ImGui::TableSetColumnIndex(3);

                    ImGui::Text("No preview");
                    
                }
                };

            // Display assets from each storage
            displayAssets(registry.GetTextureStorage(), "Texture");
            displayAssets(registry.GetModelStorage(), "Model");
            displayAssets(registry.GetMaterialStorage(), "Material");
            displayAssets(registry.GetShaderStorage(), "Shader");
            displayAssets(registry.GetFontStorage(), "Font");

            ImGui::EndTable();
        }
    }
    ImGui::End();
}

void EditorLayer::ShowRenderingSettings(bool* p_open)
{
    if (ImGui::Begin("Rendering settings", p_open))
    {
        static int numFrames = 1200;
        static std::vector<float> frameTimes(numFrames);
        static std::vector<float> frameRates(numFrames);
        static int currentIndex = 0;
        static float totalFrameTime = 0.0f;

        float deltaTime = Application::Get()->GetWindow()->GetDeltaTime();

        totalFrameTime -= frameTimes[currentIndex];
        frameTimes[currentIndex] = deltaTime;
        totalFrameTime += deltaTime;

        currentIndex = (currentIndex + 1) % numFrames;

        float frameRate = 1.0f / deltaTime;
        frameRates[currentIndex] = frameRate;

        float averageFrameTime = totalFrameTime / numFrames;
        float totalFrameRate = 0.0f;
        for (float rate : frameRates) 
        {
            totalFrameRate += rate;
        }
        float averageFrameRate = totalFrameRate / numFrames;


        ImGui::TextColored(ImVec4(0.05f, 1, 0.1f, 1), "Stats");
        ImGui::PlotLines("Frame Times (ms)", frameTimes.data(), numFrames, currentIndex, nullptr, 0.0f, 50, ImVec2(80, 30));
        ImGui::SameLine();
        ImGui::PlotLines("Frame Rates (FPS)", frameRates.data(), numFrames, currentIndex, nullptr, 0.0f, 1000, ImVec2(80, 30));

        // Delta Time
        ImGui::Text("Delta Time: ");
        ImGui::SameLine();
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 0.0f, 1.0f)); // Set color to yellow
        ImGui::Text("%.3f ms", deltaTime * 1000.0f);
        ImGui::PopStyleColor();

        // Average Frame Time
        ImGui::Text("Average Frame Time: ");
        ImGui::SameLine();
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 1.0f, 0.0f, 1.0f)); // Set color to green
        ImGui::Text("%.3f ms", averageFrameTime * 1000.0f);
        ImGui::PopStyleColor();

        // Average Frame Rate
        ImGui::Text("Average Frame Rate: ");
        ImGui::SameLine();
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 0.5f, 1.0f, 1.0f)); // Set color to light blue
        ImGui::Text("%.2f FPS", averageFrameRate);
        ImGui::PopStyleColor();

        ImGui::Separator();

        // Grid Size
        ImGui::SliderFloat("Grid Size", &Renderer::s_GridSettings.gridSize, 10.0f, 500.0f);

        // Grid Cell Size
        ImGui::SliderFloat("Grid Cell Size", &Renderer::s_GridSettings.gridCellSize, 0.1f, 10.0f);

        // Grid Color Thin
        ImGui::ColorEdit4("Grid Color Thin", &Renderer::s_GridSettings.gridColorThin.r);

        // Grid Color Thick
        ImGui::ColorEdit4("Grid Color Thick", &Renderer::s_GridSettings.gridColorThick.r);

        // Min Pixels Between Cells
        ImGui::SliderFloat("Grid Min Pixels Between Cells", &Renderer::s_GridSettings.gridMinPixelsBetweenCells, 1.0f, 10.0f);
    }
    ImGui::End();
}

void EditorLayer::OnEvent(Core::Events::Event* event)
{
    // TODO: EVENT SYSTEM !!!
    if (event->GetType() == "KeyPressedEvent") // Ugly
    {
        if (auto keyEvent = dynamic_cast<Events::KeyPressedEvent*>(event))
        {
            if (!ImGuizmo::IsUsing())
            {
                if (keyEvent->key_code == GLFW_KEY_G) m_GizmoOperation = ImGuizmo::OPERATION::TRANSLATE;
                if (keyEvent->key_code == GLFW_KEY_R) m_GizmoOperation = ImGuizmo::OPERATION::ROTATE;
                if (keyEvent->key_code == GLFW_KEY_S) m_GizmoOperation = ImGuizmo::OPERATION::SCALE;
            }
        }
    }
}

void EditorLayer::OnDettach()
{
}

void EditorLayer::DrawViewportImGui()
{
}