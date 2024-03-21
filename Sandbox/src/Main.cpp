#include <iostream>
#include "Core/Window.h"

using namespace Core;

int main()
{
    Window* window = new Window(1080, 720, "Hello, abstraction!");
    
    /* Loop until the user closes the window */
    while (!window->ShouldClose())
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        glBegin(GL_TRIANGLES);
        glVertex2f(-0.5f, -0.5f);
        glVertex2f( 0.0f,  0.5f);
        glVertex2f( 0.5f, -0.5f);
        glEnd();

        window->Update();
    }

    window->Close();

    return 0;
}