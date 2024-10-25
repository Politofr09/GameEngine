#include <iostream>
#include <imgui/imgui.h>
#include <imgui/IconsFontAwesome5.h>
#include <tinyfiledialogs/tinyfiledialogs.h>
#include <filesystem>

#include "EditorLayer.h"
#include "Core/Keyboard.h"
#include "Core/Mouse.h"
#include "Core/ImGuiLayer.h"
#include "Core/AssetRegistry.h"
#include "Events/Event.h"

#include "Renderer/Renderer.h"
#include "Renderer/Renderer2D.h"
#include "Renderer/TextRenderer.h"
#include "Core/Application.h"

#include "Ecs/ECS.h"

#include "CommonUI.h"

using namespace Core::Gfx;

// Temporary
// Will be soon removed (finally!)
Core::AssetHandle modelhandle1;
Core::AssetHandle modelhandle2;
Core::AssetHandle fonthandle;
Light light;
Core::AssetHandle texturehandle;

void EditorLayer::OnAttach()
{
    // Setup event
	Core::Events::Dispatcher::Subscribe(std::bind(&EditorLayer::OnEvent, this, std::placeholders::_1));
	
    // Setup OpenGL renderer
    Renderer::Init();
    Renderer2D::Init();
    TextRenderer::Init();

    // This shit is all (hopefully) temporary. Once serialization is *fully* added all of this manual shitty asset loading will be gone.
    //modelhandle1 = Model::Create("res/Monkey.obj", "3DMonkey");
    //modelhandle2 = Model::Create("res/bunny.obj", "Bunny");

    //texturehandle = Texture::Create("res/water.jpg", "WaterTexture");
    //
    //fonthandle = Font::Create("res/ocr-a-extended.ttf", "OCR-Font", 100);

    /**** Setup the camera(s) ****/
    m_Cam.SetAspectRatio((float)m_Application->GetWindow()->GetWidth() / m_Application->GetWindow()->GetHeight());
    m_Cam2d = Core::Gfx::OrthographicCamera(0.0f, (float)m_Application->GetWindow()->GetWidth(), (float)m_Application->GetWindow()->GetHeight(), 0.0f);

    m_FrameBuffer = FrameBuffer(m_Application->GetWindow()->GetWidth(), m_Application->GetWindow()->GetHeight());

    light = Light();
}

static int FONT_SCALE = 3;

void EditorLayer::OnUpdate()
{
    // UpdateCameraController();
    //shader->SetFloat("uTime", (float)glfwGetTime());
    m_FrameBuffer.Bind();
    Renderer::OnViewportResize(m_FrameBuffer.GetWidth(), m_FrameBuffer.GetHeight());
    Renderer::SetBackgroundColor({ 0.2f, 0.3f, 0.3f });
    Renderer::Clear();
    Renderer::Begin(m_Cam);
    {
        Renderer::Clear();

        glm::mat4 trans = glm::mat4(1);

        //auto& model1 = *OPENED_PROJECT.GetRegistry().Get<Model>(modelhandle1);
        //Renderer::DrawModel(model1, trans);

        //trans = glm::translate(trans, glm::vec3(10, 0, 0));

        //auto& model2 = *OPENED_PROJECT.GetRegistry().Get<Model>(modelhandle2);
        //Renderer::DrawModel(model2, trans);

        // Eventually this will be more complex, we will have a list of scenes that we can edit with the editor layer
        Renderer::RenderScene(OPENED_PROJECT.GetScene());
    }
    Renderer::End();

    //TextRenderer::Begin(m_Cam2d);
    //{
    //    auto& font = *OPENED_PROJECT.GetRegistry().Get<Font>(fonthandle);

    //    std::string message = "Frametime " + std::to_string(m_Application->GetWindow()->GetDeltaTime()) + "ms";

    //    glm::ivec2 offset = font.MeasureText(message, FONT_SCALE);
    //    float x = m_FrameBuffer.GetWidth() - offset.x - 10;
    //    float y = offset.y + 10;
    //    TextRenderer::DrawTextEx(font, message, x, y, FONT_SCALE, glm::vec3(sin(glfwGetTime()), cos(glfwGetTime()), 1));
    //}
    //TextRenderer::End();
    m_FrameBuffer.UnBind();
}

void EditorLayer::OnImGuiRender()
{
    // ImGui content
    //ImGui::Begin("Menubar", (bool*)0, ImGuiWindowFlags_MenuBar);
    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Exit"))
            {
                m_Application->GetWindow()->Close();
            }
            if (ImGui::MenuItem("Open"))
            {
                const char* filterPatterns[1] = { "*.yaml" };
                const char* path = tinyfd_openFileDialog("Select a project file", "", 1, filterPatterns, NULL, 0);

                if (path)
                {
                    m_Application->LoadProject(path);
                }
            }
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("View"))
        {
            ImGui::MenuItem("Toggle Camera Controll", NULL, &showCameraControl);
            ImGui::MenuItem("Toggle Theme Switcher", NULL, &showThemeSwitcher);
            ImGui::MenuItem("Toggle Asset Registry", NULL, &showAssetRegistry);
            ImGui::MenuItem("Toggle Material Editor", NULL, &showMaterialEditor);
            ImGui::MenuItem("Toggle Statistics", NULL, &showRenderingSettings);
            ImGui::MenuItem("Toggle ECS panel", NULL, &showECSPanel);
            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }

    ImGui::ShowDemoWindow();

    if (showCameraControl)      ShowCameraControlImgui(&showCameraControl);
    if (showAssetRegistry)      ShowAssetRegistry(&showAssetRegistry);
    if (showMaterialEditor)     ShowMaterialEditor(&showMaterialEditor);
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
            m_FrameBuffer.Resize((int)availableSpace.x, (int)availableSpace.y);
            m_Cam.OnViewportResize((int)availableSpace.x, (int)availableSpace.y);
            m_Cam2d.OnViewportResize((int)availableSpace.x, (int)availableSpace.y);
        }

        m_FrameBuffer.SetPosition(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y + 6);

        ImVec2 min = ImGui::GetCursorScreenPos();
        ImVec2 max = ImVec2(min.x + availableSpace.x, min.y + availableSpace.y);
        ImGui::GetWindowDrawList()->AddRect(min, max, ImGui::ColorConvertFloat4ToU32(ImGui::GetStyleColorVec4(ImGuiCol_WindowBg)), 6.0f, ImDrawFlags_RoundCornersAll, 6.0f);

        ImGui::BeginChild("Viewport", availableSpace, false, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoMove);
        DisplayRoundedImage((ImTextureID)m_FrameBuffer.GetTextureID(), ImVec2(m_FrameBuffer.GetWidth(), m_FrameBuffer.GetHeight()), 6.0f, ImVec2(0, 1), ImVec2(1, 0));
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
    //if (!p_open) return;

    if (ImGui::Begin("Switch perspective", p_open))
    {

        static float in_FOV = 75.0F;
        if (ImGui::SliderFloat("FOV", &in_FOV, 30.0f, 120.0f))
        {
            float aspectRatio = m_FrameBuffer.GetAspectRatio();
            m_Cam.SetProjectionMatrix(glm::perspective(glm::radians(in_FOV), aspectRatio, 0.01f, 1000.0f));
            //shader->SetMatrix("uProjection", m_Cam.GetProjectionMatrix());
        }

        ImGui::SameLine();
        if (ImGui::Button("Reset"))
        {
            in_FOV = 75.0F;
            float aspectRatio = m_FrameBuffer.GetAspectRatio();
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
        float aspectRatio = (float)m_Application->GetWindow()->GetWidth() / m_Application->GetWindow()->GetHeight();

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
            Core::ImGuiLayer::SwitchTheme(!Core::ImGuiLayer::IsDarkThemed());
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
        ImGui::Text("Number of assets: %i", OPENED_PROJECT.GetRegistry().GetAllResources().size());
        if (ImGui::BeginTable("Resource Table", 4, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg))
        {
            // Set up column headers
            ImGui::TableSetupColumn("Asset Name", ImGuiTableColumnFlags_WidthStretch);
            ImGui::TableSetupColumn("Asset Type", ImGuiTableColumnFlags_WidthStretch);
            ImGui::TableSetupColumn("Asset ID", ImGuiTableColumnFlags_WidthStretch);
            ImGui::TableSetupColumn("Preview", ImGuiTableColumnFlags_WidthFixed, 100.0f);
            ImGui::TableHeadersRow();

            // Populate the table with resource data
            for (const auto& pair : OPENED_PROJECT.GetRegistry().GetAllResources())
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
                    Texture* tex = OPENED_PROJECT.GetRegistry().Get<Texture>(assetID);
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

void EditorLayer::ShowMaterialEditor(bool* p_open)
{
    //if (!p_open) return;

    if (ImGui::Begin("Material editor", p_open))
    {
        Model& model = *OPENED_PROJECT.GetRegistry().Get<Model>(modelhandle1);
        Material& material = *OPENED_PROJECT.GetRegistry().Get<Material>(model.GetMaterialHandle());
        
        // Shader Type Selector
        const char* shaderTypes[] = { "None", "FlatShading", "PhongShading" }; // Update with your actual shader types
        static int currentShaderType = static_cast<int>(material.m_ShaderType);
        if (ImGui::Combo("Shader Type", &currentShaderType, shaderTypes, IM_ARRAYSIZE(shaderTypes)))
        {
            material.m_ShaderType = static_cast<ShaderType>(currentShaderType);
        }

        // Color Slider
        ImGui::ColorEdit3("Color", &material.Color[0]);

        // Ambient Slider
        ImGui::ColorEdit3("Ambient", &material.Ambient[0]);

        // Diffuse Slider
        ImGui::ColorEdit3("Diffuse", &material.Diffuse[0]);

        // Specular Slider
        ImGui::ColorEdit3("Specular", &material.Specular[0]);
        
        // Shininess Slider
        ImGui::SliderFloat("Shininess", &material.Shininess, 0.01f, 128.0f);

        // Light settings
        ImGui::SliderFloat3("Light Position", glm::value_ptr(light.Position), -500.0f, 500.0f);
        //std::cout << light.Position.x << std::endl;
        Renderer::SetLight(light);
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

        float deltaTime = m_Application->GetWindow()->GetDeltaTime();

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

void EditorLayer::ShowECSPanel(bool* p_open)
{
    using namespace Core::Ecs;
    if (ImGui::Begin("ECS", p_open))
    {
        ECS& registry = OPENED_PROJECT.GetScene().GetRegistry();

        registry.each([&](ECS::EntityID e) 
        {
            Entity entity{ e, registry };
            ImGui::Text("Entity %u", entity.GetID());
            if (ImGui::TreeNode(entity.GetName().c_str()))
            {
                // Show the components of the entity
                ImGui::Text("Components:");

                if (entity.HasComponent<TransformComponent>() && ImGui::TreeNode("TransformComponent"))
                {
                    auto& t = entity.GetComponent<TransformComponent>();

                    // Use sliders for position, rotation, and scale
                    ImGui::SliderFloat3("Position", &t.Position.x, -10.0f, 10.0f);
                    ImGui::SliderFloat3("Rotation", &t.Rotation.x, -180.0f, 180.0f);
                    ImGui::SliderFloat3("Scale", &t.Scale.x, 0.1f, 10.0f);
                    
                    ImGui::TreePop();
                }

                if (entity.HasComponent<ModelComponent>() && ImGui::TreeNode("ModelComponent"))
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

                    auto model = OPENED_PROJECT.GetRegistry().Get<Core::Gfx::Model>(m.ModelHandle);
                    auto material = OPENED_PROJECT.GetRegistry().Get<Core::Gfx::Material>(model->GetMaterialHandle());

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
                auto& ECS_Registry = OPENED_PROJECT.GetScene().GetRegistry();
                if (ImGui::MenuItem("TransformComponent"))
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

                if (ImGui::MenuItem("ModelComponent"))
                {
                    if (!ECS_Registry.HasComponent<ModelComponent>(e))
                    {
                        ECS_Registry.AddComponent<ModelComponent>(e, ModelComponent{ 0 });
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