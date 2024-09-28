#include <iostream>
#include <imgui/imgui.h>
#include <imgui/IconsFontAwesome5.h>
#include <tinyfiledialogs/tinyfiledialogs.h>

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

#include "CommonUI.h"

using namespace Core::Gfx;

Model model1;
Model model2;
Font* font;
Light light;
Texture* texture;

void EditorLayer::OnAttach()
{
    // Setup event
	Core::Events::Dispatcher::Subscribe(std::bind(&EditorLayer::OnEvent, this, std::placeholders::_1));
	
    // Setup OpenGL renderer
    Renderer::Init(m_Application->GetProject().registry);
    Renderer2D::Init();
    TextRenderer::Init();

    //shader = &Shader::Create("res/FlatShader.glsl", "MyFlatShader");
    //m_Application->GetProject().registry.Track(shader);

    //model1 = Model::LoadModel("res/Monkey.obj");
    model1 = Model::Create(m_Application->GetProject().registry, "res/Monkey.obj", "monkey3D");

    model2 = Model::Create(m_Application->GetProject().registry, "res/bunny.obj", "bunny");

    model2.GetMaterial().m_ShaderType = ShaderType::PhongShading;
    // TODO: Track model, mesh, material assets.

    /**** Setup the camera(s) ****/
    m_Cam.SetAspectRatio((float)m_Application->GetWindow()->GetWidth() / m_Application->GetWindow()->GetHeight());
    m_Cam2d = Core::Gfx::OrthographicCamera(0.0f, (float)m_Application->GetWindow()->GetWidth(), (float)m_Application->GetWindow()->GetHeight(), 0.0f);

    texture = &Texture::Create(m_Application->GetProject().registry, "res/water.jpg");
    m_Application->GetProject().registry.Track(texture);


    font = &Font::Create(m_Application->GetProject().registry, "res/ocr-a-extended.ttf", 100, "OCR-Font");
    
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
    Renderer::SetBackgroundColor({0.2f, 0.3f, 0.3f});
    Renderer::Clear();
    Renderer::Begin(m_Cam);
    {
        Renderer::Clear();

        glm::mat4 trans = glm::mat4(1);

        Renderer::DrawModel(model1, trans);
        
        trans = glm::translate(trans, glm::vec3(10, 0, 0)); 
        Renderer::DrawModel(model2, trans);
    }
    Renderer::End();

    TextRenderer::Begin(m_Cam2d);
    {
        std::string message = "Frametime " + std::to_string(m_Application->GetWindow()->GetDeltaTime()) + "ms";
        
        glm::ivec2 offset = font->MeasureText(message, FONT_SCALE);
        float x = m_FrameBuffer.GetWidth() - offset.x - 10;
        float y = offset.y + 10;
        TextRenderer::DrawTextEx(*font, message, x, y, FONT_SCALE, glm::vec3(sin(glfwGetTime()), cos(glfwGetTime()), 1));
    }
    TextRenderer::End();
    m_FrameBuffer.UnBind();
}

void EditorLayer::OnImGuiRender()
{
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

            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }

    // ImGui content
    ImGui::ShowDemoWindow();

    if (showCameraControl)      ShowCameraControlImgui(&showCameraControl);
    if (showAssetRegistry)      ShowAssetRegistry(&showAssetRegistry);
    if (showMaterialEditor)     ShowMaterialEditor(&showMaterialEditor);
    if (showThemeSwitcher)      ShowThemeSwitcher(&showThemeSwitcher);
    if (showRenderingSettings)  ShowRenderingSettings(&showRenderingSettings);

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
        ImGui::Text("Number of assets: %i", m_Application->GetProject().registry.GetAllResources().size());
        if (ImGui::BeginTable("Resource Table", 4, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg))
        {
            // Set up column headers
            ImGui::TableSetupColumn("Asset Name", ImGuiTableColumnFlags_WidthStretch);
            ImGui::TableSetupColumn("Asset Type", ImGuiTableColumnFlags_WidthStretch);
            ImGui::TableSetupColumn("Asset ID", ImGuiTableColumnFlags_WidthStretch);
            ImGui::TableSetupColumn("Preview", ImGuiTableColumnFlags_WidthFixed, 100.0f);
            ImGui::TableHeadersRow();

            // Populate the table with resource data
            for (const auto& resource : m_Application->GetProject().registry.GetAllResources())
            {
                ImGui::TableNextRow();

                // Asset Name
                ImGui::TableSetColumnIndex(0);
                ImGui::TextUnformatted(resource->GetName().c_str());

                // Asset Type
                ImGui::TableSetColumnIndex(1);
                ImGui::TextUnformatted(resource->GetType());

                // Asset ID
                ImGui::TableSetColumnIndex(2);
                ImGui::Text("%x", resource->ID);

                // Preview
                ImGui::TableSetColumnIndex(3);
                if (std::string(resource->GetType()) == "Texture")
                {
                    Texture* tex = dynamic_cast<Texture*>(resource);
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
        Material* material = &model2.GetMaterial();

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

        // Light settings
        ImGui::SliderFloat3("Light Position", glm::value_ptr(light.Position), -500.0f, 500.0f);
        std::cout << light.Position.x << std::endl;
        Renderer::SetLight(light);
    }
    ImGui::End();
}

void EditorLayer::ShowRenderingSettings(bool* p_open)
{
    if (ImGui::Begin("Rendering settings", p_open))
    {
        static int numFrames = 100;
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
        ImGui::PlotLines("Frame Times (ms)", frameTimes.data(), numFrames, currentIndex, nullptr, 0.0f, *std::max_element(frameTimes.begin(), frameTimes.end()), ImVec2(80, 30));
        ImGui::SameLine();
        ImGui::PlotLines("Frame Rates (FPS)", frameRates.data(), numFrames, currentIndex, nullptr, 0.0f, *std::max_element(frameRates.begin(), frameRates.end()), ImVec2(80, 30));

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



void EditorLayer::OnEvent(Core::Events::Event* event)
{
}

void EditorLayer::OnDettach()
{
}

void EditorLayer::DrawViewportImGui()
{
}