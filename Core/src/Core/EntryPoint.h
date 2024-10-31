#pragma once

#include "Application.h"

extern Core::Application* Core::CreateApplication(ApplicationCommandLineArgs args);

// Entry point stuff
int main(int argc, char** argv)
{
    Core::ApplicationCommandLineArgs args;
    args.Count = argc;
    args.Args = argv;

    // Create application instance and run it
    auto application = Core::CreateApplication(args);
    application->Run();
    delete application;

    return 0;
}