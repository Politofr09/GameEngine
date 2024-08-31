#include "EditorLayer.h"
#include "TilemapLayer.h"
#include "Core/Application.h"
#include <memory>

using namespace Core;

int main()                                                                      
{
    std::unique_ptr<Application> app = std::make_unique<Application>();

    EditorLayer* editorLayer = new EditorLayer(app->GetWindow());
    app->PushLayer(editorLayer);

    TilemapLayer* tilemapLayer = new TilemapLayer(app->GetWindow());
    app->PushLayer(tilemapLayer);

    app->Run();

    return 0;
}