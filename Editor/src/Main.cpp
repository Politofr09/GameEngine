#include "SandboxLayer.h"
#include "Core/Application.h"
#include <memory>

using namespace Core;

int main()                                                                      
{
    std::unique_ptr<Application> app = std::make_unique<Application>();

    SandboxLayer* sandboxLayer = new SandboxLayer(app->GetWindow());
    app->PushLayer(sandboxLayer);

    app->Run();

    return 0;
}