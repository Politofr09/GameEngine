#include <iostream>
#include <imgui/imgui.h>
#include <imgui/IconsFontAwesome5.h>
#include <tinyfiledialogs/tinyfiledialogs.h>
#include <filesystem>

#include "EditorLayer.h"
#include "ImGuiLayer.h"

#include "Core/Keyboard.h"
#include "Core/Mouse.h"
#include "Core/AssetRegistry.h"
#include "Core/Application.h"
#include "Core/Instrumentor.h"
#include "Events/Event.h"

#include "Renderer/Renderer.h"
#include "Renderer/Renderer2D.h"
#include "Renderer/TextRenderer.h"
#include "Core/Application.h"

#include "Ecs/ECS.h"

#include "CommonUI.h"

using namespace Core::Gfx;
using namespace Core;

void EditorLayer::OnAttach()
{
    CORE_PROFILE_FUNCTION();

    // Setup event
	Events::Dispatcher::Subscribe(std::bind(&EditorLayer::OnEvent, this, std::placeholders::_1));
	
    int width = Application::Get()->GetWindow()->GetWidth();
    int height = Application::Get()->GetWindow()->GetHeight();

    // Setup *OpenGL* renderer
    Renderer::Init(width, height);
    Renderer2D::Init(width, height);
    TextRenderer::Init();

    /**** Setup the camera(s) ****/
    m_Cam.SetAspectRatio((float)width / height);
    m_Cam2d = Core::Gfx::OrthographicCamera(0.0f, (float)width, (float)height, 0.0f);
}

static int FONT_SCALE = 3;

void EditorLayer::OnUpdate()
{
    // UpdateCameraController();
    //shader->SetFloat("uTime", (float)glfwGetTime());
    Renderer::SetBackgroundColor({ 0.2f, 0.3f, 0.3f });
    Renderer::Clear();
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
        Renderer::Clear();

        auto& registry = Application::Get()->GetCurrentProject().GetScene().GetRegistry();
        auto& view = registry.GetView<Ecs::TransformComponent, Ecs::SpriteComponent>();
        for (auto e : view)
        {
            auto& t = registry.GetComponent<Ecs::TransformComponent>(e);
            auto& s = registry.GetComponent<Ecs::SpriteComponent>(e);

            if (s.TextureHandle)
            {
                Texture* texture = Application::Get()->GetCurrentProject().GetRegistry().Get<Texture>(s.TextureHandle);

                Renderer2D::DrawQuadTextured(glm::vec3(t.Position), glm::vec2(t.Scale.x, t.Scale.y), s.Color,
                    *texture
                );
            }
            else
            {
                Renderer2D::DrawQuad(glm::vec3(t.Position), glm::vec2(t.Scale.x, t.Scale.y), s.Color);
            }
        }
    }
    Renderer2D::End();
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
            ImGui::MenuItem("Toggle ECS panel", NULL, &showECSPanel);
            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }

    ImGui::ShowDemoWindow();

    if (showCameraControl)      ShowCameraControlImgui(&showCameraControl);
    if (showAssetRegistry)      ShowAssetRegistry(&showAssetRegistry);
    if (showThemeSwitcher)      ShowThemeSwitcher(&showThemeSwitcher);
    if (showECSPanel)           ShowECSPanel(&showECSPanel);
    if (showRenderingSettings)  ShowRenderingSettings(&showRenderingSettings);
    if (showContentBrowser)     ShowContentBrowser(&showContentBrowser);

    static ImVec2 availableSpace = ImVec2();
    ImGui::GetStyle().WindowPadding = { 1.5f, 1.5f };
    if (ImGui::Begin("VIEWPORT", 0, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse))
    {
        if (availableSpace.x != ImGui::GetContentRegionAvail().x || availableSpace.y != ImGui::GetContentRegionAvail().y)
        {
            availableSpace = ImGui::GetContentRegionAvail();
            Renderer::OnViewportResize((int)availableSpace.x, (int)availableSpace.y);
            Renderer2D::OnViewportResize((int)availableSpace.x, (int)availableSpace.y);
            m_Cam.OnViewportResize((int)availableSpace.x, (int)availableSpace.y);
            m_Cam2d.OnViewportResize((int)availableSpace.x, (int)availableSpace.y);
        }

        ImVec2 min = ImGui::GetCursorScreenPos();
        ImVec2 max = ImVec2(min.x + availableSpace.x, min.y + availableSpace.y);
        ImGui::GetWindowDrawList()->AddRect(min, max, ImGui::ColorConvertFloat4ToU32(ImGui::GetStyleColorVec4(ImGuiCol_WindowBg)), 6.0f, ImDrawFlags_RoundCornersAll, 6.0f);

        FrameBuffer fb = Renderer2D::GetFramebuffer();

        ImGui::BeginChild("Viewport", availableSpace, false, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoMove);
        DisplayRoundedImage((ImTextureID)fb.GetTextureID(), ImVec2(fb.GetWidth(), fb.GetHeight()), 6.0f, ImVec2(0, 1), ImVec2(1, 0));
        if (ImGui::IsWindowHovered())
        {
            m_Cam.Move();
        }
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
            float aspectRatio = Renderer::GetFramebuffer().GetAspectRatio();
            m_Cam.SetProjectionMatrix(glm::perspective(glm::radians(in_FOV), aspectRatio, 0.01f, 1000.0f));
            //shader->SetMatrix("uProjection", m_Cam.GetProjectionMatrix());
        }

        ImGui::SameLine();
        if (ImGui::Button("Reset"))
        {
            in_FOV = 75.0F;
            float aspectRatio = Renderer::GetFramebuffer().GetAspectRatio();
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
    //if (!p_open) return;

    if (ImGui::Begin("Asset Registry", p_open))
    {
        ImGui::Text("Number of assets: %i", Application::Get()->GetCurrentProject().GetRegistry().GetAllResources().size());
        if (ImGui::BeginTable("Resource Table", 4, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg))
        {
            // Set up column headers
            ImGui::TableSetupColumn("Asset Name", ImGuiTableColumnFlags_WidthStretch);
            ImGui::TableSetupColumn("Asset Type", ImGuiTableColumnFlags_WidthStretch);
            ImGui::TableSetupColumn("Asset ID", ImGuiTableColumnFlags_WidthStretch);
            ImGui::TableSetupColumn("Preview", ImGuiTableColumnFlags_WidthFixed, 100.0f);
            ImGui::TableHeadersRow();

            // Populate the table with resource data
            for (const auto& pair : Application::Get()->GetCurrentProject().GetRegistry().GetAllResources())
            {
                uint64_t assetID = pair.first;

                ImGui::TableNextRow();

                // Asset Name
                ImGui::TableSetColumnIndex(0);
                ImGui::Button(pair.second->GetName().c_str());
                //ImGui::PushID(static_cast<int>(assetID));

                // Drag and drop asset ids
                if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID))
                {
                    std::string item_name = "ASSET_DND_" + std::string(pair.second->GetType());
                    ImGui::SetDragDropPayload(item_name.c_str(), &assetID, sizeof(assetID));

                    // Display the asset ID in the UI
                    ImGui::Text("%s: %llu", pair.second->GetType(), assetID); // Use %llu for printing uint64_t

                    ImGui::EndDragDropSource();
                }
                //ImGui::PopID();

                // Asset Type
                ImGui::TableSetColumnIndex(1);
                ImGui::TextUnformatted(pair.second->GetType());
                // Asset ID
                ImGui::TableSetColumnIndex(2);
                ImGui::Text("%llu", assetID);

                // Preview
                ImGui::TableSetColumnIndex(3);
                if (std::string(pair.second->GetType()) == "Texture")
                {
                    Texture* tex = Application::Get()->GetCurrentProject().GetRegistry().Get<Texture>(assetID);
                    ImGui::Image((ImTextureID)(intptr_t)tex->GetID(), { 100.0f, 100.0f });
                }
                else
                {
                    ImGui::Text("No preview");
                }

            }
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
        for (float rate : frameRates) {
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
        
        // Rendering settings
        static bool previousFaceCulling = true;
        static bool faceCulling = true;

        ImGui::Checkbox("Face culling", &faceCulling);

        // Check if face culling state changed
        if (faceCulling != previousFaceCulling) {
            if (faceCulling) {
                Renderer::EnableCulling();
            }
            else {
                Renderer::DisableCulling();
            }
            previousFaceCulling = faceCulling; // Update the previous state
        }

        static bool previousDepthTesting = true;
        static bool depthTesting = true;

        ImGui::SameLine();
        ImGui::Checkbox("Depth testing", &depthTesting);

        // Check if face depth testing state changed
        if (depthTesting != previousDepthTesting) {
            if (depthTesting) {
                Renderer::EnableDepthTesting();
            }
            else {
                Renderer::DisableDepthTesting();
            }
            previousDepthTesting = depthTesting; // Update the previous state
        }
    
        static bool previousWireframeMode = true;
        static bool wireFrameMode = true;

        ImGui::SameLine();
        ImGui::Checkbox("Wireframe mode", &wireFrameMode);

        // Check if face depth testing state changed
        if (wireFrameMode != previousWireframeMode) {
            if (wireFrameMode) {
                Renderer::EnableWireframeMode();
            }
            else {
                Renderer::DisableWireframeMode();
            }
            previousWireframeMode = wireFrameMode; // Update the previous state
        }
    }
    ImGui::End();
}

// We have some serious work to do on this specific panel...
void EditorLayer::ShowECSPanel(bool* p_open)
{
    using namespace Core::Ecs;
    if (ImGui::Begin("ECS", p_open))
    {
        ECS& registry = Application::Get()->GetCurrentProject().GetScene().GetRegistry();

        if (ImGui::Button("Add Entity"))
        {
            registry.AddComponent<NameComponent>(registry.CreateEntity(), NameComponent{ "Entity" });
        }

        registry.each([&](ECS::EntityID e) 
        {
            Entity entity{ e, registry };
            std::string nodeName = entity.GetName() + " (" + std::to_string(e) + ")";
            if (ImGui::TreeNode(nodeName.c_str()))
            {
                // Show the components of the entity
                ImGui::Text("Components:");

                if (entity.HasComponent<TransformComponent>() && ImGui::TreeNode("Transform"))
                {
                    auto& t = entity.GetComponent<TransformComponent>();

                    // Use sliders for position, rotation, and scale
                    ImGui::DragFloat3("Position", &t.Position.x, 0.1f);
                    ImGui::DragFloat3("Rotation", &t.Rotation.x);
                    ImGui::DragFloat3("Scale", &t.Scale.x, 0.1f);

                    ImGui::TreePop();
                }

                if (entity.HasComponent<ModelComponent>() && ImGui::TreeNode("Model"))
                {
                    auto& m = entity.GetComponent<ModelComponent>();

                    ImGui::Text("Model: %llu", m.ModelHandle);

                    // Accept payload from drag and drop imgui (only model ids)
                    if (ImGui::BeginDragDropTarget())
                    {
                        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ASSET_DND_Model"))
                        {
                            Core::AssetHandle handle = *(uint64_t*)payload->Data;
                            m.ModelHandle = handle;
                        }
                        ImGui::EndDragDropTarget();
                    }

                    auto model = Application::Get()->GetCurrentProject().GetRegistry().Get<Core::Gfx::Model>(m.ModelHandle);
                    auto material = Application::Get()->GetCurrentProject().GetRegistry().Get<Core::Gfx::Material>(model->GetMaterialHandle());

                    ImGui::Text("Material: %llu", model->GetMaterialHandle());

                    // Accept payload from drag and drop imgui (only model ids)
                    if (ImGui::BeginDragDropTarget() && m.ModelHandle != 0)
                    {
                        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ASSET_DND_Material"))
                        {
                            Core::AssetHandle handle = *(uint64_t*)payload->Data;
                            model->SetMaterialHandle(handle);
                        }
                        ImGui::EndDragDropTarget();
                    }

                    if (ImGui::TreeNode("Material editor") && m.ModelHandle != 0)
                    {
                        // Shader Type Selector
                        const char* shaderTypes[] = { "None", "FlatShading", "PhongShading" }; // Update with your actual shader types
                        static int currentShaderType = static_cast<int>(material->m_ShaderType);
                        if (ImGui::Combo("Shader Type", &currentShaderType, shaderTypes, IM_ARRAYSIZE(shaderTypes)))
                        {
                            material->m_ShaderType = static_cast<ShaderType>(currentShaderType);
                        }

                        // Color Slider
                        ImGui::ColorEdit3("Color", &material->Color[0]);

                        // Ambient Slider
                        ImGui::ColorEdit3("Ambient", &material->Ambient[0]);

                        // Diffuse Slider
                        ImGui::ColorEdit3("Diffuse", &material->Diffuse[0]);

                        // Specular Slider
                        ImGui::ColorEdit3("Specular", &material->Specular[0]);

                        // Shininess Slider
                        ImGui::SliderFloat("Shininess", &material->Shininess, 0.01f, 128.0f);
                        
                        // Textures
                        ImGui::Text("DiffuseTexture: %llu", material->DiffuseTextureHandle);
                        if (ImGui::BeginDragDropTarget())
                        {
                            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ASSET_DND_Texture"))
                            {
                                Core::AssetHandle handle = *(uint64_t*)payload->Data;
                                material->DiffuseTextureHandle = handle;
                            }
                            ImGui::EndDragDropTarget();
                        }

                        ImGui::TreePop();
                    }

                    ImGui::TreePop();
                }
                
                if (entity.HasComponent<SpriteComponent>() && ImGui::TreeNode("Sprite"))
                {
                    auto& s = entity.GetComponent<SpriteComponent>();
                    ImGui::Text("Texture: %llu", s.TextureHandle);
                   
                    // Accept payload from drag and drop imgui (only model ids)
                    if (ImGui::BeginDragDropTarget())
                    {
                        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ASSET_DND_Texture"))
                        {
                            Core::AssetHandle handle = *(uint64_t*)payload->Data;
                            s.TextureHandle = handle;
                        }
                        ImGui::EndDragDropTarget();
                    }
                    
                    // Use sliders for position, rotation, and scale
                    ImGui::ColorEdit4("Color", &s.Color.x);

                    ImGui::TreePop();
                }

                ImGui::TreePop();
                ImGui::SetCursorPosX(25);
                if (ImGui::Button("Add Component", ImVec2(150.0f, 40.0f)))
                {
                    ImVec2 button_pos = ImGui::GetItemRectMin();  // Get top-left corner of button
                    ImVec2 button_size = ImGui::GetItemRectSize(); // Get button size
                    ImGui::SetNextWindowPos(ImVec2(button_pos.x, button_pos.y + button_size.y)); // Set position directly below the button
                    ImGui::OpenPopup("AddComponentPopup");
                }

            }

            // Dropdown menu logic
            if (ImGui::BeginPopup("AddComponentPopup"))
            {
                auto& ECS_Registry = Application::Get()->GetCurrentProject().GetScene().GetRegistry();
                if (ImGui::MenuItem("Transform"))
                {
                    // Add TransformComponent to the selected entity
                    if (!ECS_Registry.HasComponent<TransformComponent>(e))
                    {
                        TransformComponent transform{};
                        transform.Position = { 0.0f, 0.0f, 0.0f };
                        transform.Rotation = { 0.0f, 0.0f, 0.0f };
                        transform.Scale = { 1.0f, 1.0f, 1.0f };

                        ECS_Registry.AddComponent<TransformComponent>(e, transform);
                    }
                }

                if (ImGui::MenuItem("Model"))
                {
                    if (!ECS_Registry.HasComponent<ModelComponent>(e))
                    {
                        ECS_Registry.AddComponent<ModelComponent>(e, ModelComponent{ 0 });
                    }
                }

                if (ImGui::MenuItem("Sprite"))
                { 
                    if (!ECS_Registry.HasComponent<SpriteComponent>(e))
                    {
                        ECS_Registry.AddComponent<SpriteComponent>(e, SpriteComponent{ 0, glm::vec4(1.0f) });
                    }
                }

                ImGui::EndPopup();
            }
        });
    }
    ImGui::End();
}

void EditorLayer::ShowContentBrowser(bool* p_open)
{
    if (ImGui::Begin("Content Browser", p_open))
    {
        static char directoryBuf[128] = "res";

        ImGui::SetCursorPosX(ImGui::GetContentRegionAvail().x / 10 );

        if (ImGui::Button(ICON_FA_ARROW_LEFT " Back"))
        {
            std::string pathStr = directoryBuf;

            size_t pos = pathStr.find_last_of("/\\");

            if (pos != std::string::npos)
            {
                pathStr = pathStr.substr(0, pos);
            }

            if (pathStr.empty())
                pathStr = "res";

            std::strncpy(directoryBuf, pathStr.c_str(), sizeof(directoryBuf) - 1);
            directoryBuf[sizeof(directoryBuf) - 1] = '\0';
        }

        ImGui::SameLine();
        ImGui::InputText("Directory", directoryBuf, sizeof(directoryBuf));

        ImGui::SameLine();
        if (ImGui::Button(ICON_FA_FOLDER))
        {
            const char* dir = tinyfd_selectFolderDialog("Select a directory", "/res");
            if (dir)
            {
                std::strncpy(directoryBuf, dir, sizeof(directoryBuf) - 1);
                directoryBuf[sizeof(directoryBuf) - 1] = '\0';
            }
        }

        if (ImGui::IsItemHovered())
        {
            if (ImGui::BeginTooltip())
            {
                ImGui::Text("Select folder");
                ImGui::EndTooltip();
            }
        }

        if (std::filesystem::exists(directoryBuf))
        {
            ImGui::BeginChild("Folders", ImVec2(ImGui::GetContentRegionAvail().x / 5, ImGui::GetContentRegionAvail().y), true);
            
            // Recursive lambda for child window; only folders
            std::function<void(const char*)> FolderRecursive = [&](const char* directory)
            {
                for (const auto& entry : std::filesystem::directory_iterator(directory))
                {
                    if (entry.is_directory())
                    {
                        std::string folderName = entry.path().filename().string();
                        if (ImGui::TreeNode(folderName.c_str()))
                        {
                            FolderRecursive(entry.path().string().c_str());
                            ImGui::TreePop();
                        }
                    }
                }
            };

            FolderRecursive(directoryBuf);
            ImGui::EndChild();

            // Main thumbnail view
            ImGui::SameLine();
            ImGui::BeginChild("Content", ImVec2(0, ImGui::GetContentRegionAvail().y), true);
            
            float contentWidth = ImGui::GetContentRegionAvail().x;
            int columns = static_cast<int>(contentWidth / 100);
            if (columns < 1) columns = 1;


            ImGui::Columns(columns, nullptr, false);

            for (const auto& entry : std::filesystem::directory_iterator(directoryBuf))
            {
                std::string fileName = entry.path().filename().string();

                const char* icon = ICON_FA_QUESTION;
                if (entry.is_directory()) icon = ICON_FA_FOLDER;
                
                const std::string ext = entry.path().extension().string();
                if (ext == ".jpg" || ext == ".png") icon = ICON_FA_FILE_IMAGE;
                if (ext == ".ttf") icon = ICON_FA_FILE_ARCHIVE;
                if (ext == ".glsl") icon = ICON_FA_FILE_CODE;

                if (ImGui::Button(icon, ImVec2(100, 100)))
                {
                    std::strncpy(directoryBuf, entry.path().string().c_str(), sizeof(directoryBuf) - 1);
                    directoryBuf[sizeof(directoryBuf) - 1] = '\0';
                }

                if (ImGui::IsItemHovered())
                {
                    ImGui::BeginTooltip();
                    ImGui::Text("%s", fileName.c_str());
                    ImGui::EndTooltip();
                }

                ImGui::TextWrapped(fileName.c_str());

                ImGui::NextColumn();
            }

            ImGui::Columns(1);  // End the column layout
            ImGui::EndChild();
        }
        else
        {
            ImGui::SetCursorPosX(ImGui::GetContentRegionAvail().x / 20);
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 0, 0, 1));
            ImGui::Text("Please enter a valid directory!");
            ImGui::PopStyleColor();
        }
    }
    ImGui::End();
}


void EditorLayer::OnEvent(Core::Events::Event* event)
{
}

void EditorLayer::OnDettach()
{
}

void EditorLayer::DrawViewportImGui()
{
}