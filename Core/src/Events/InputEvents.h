#include "Event.h"

#pragma once 

namespace Core::Events
{

    /**** Input: keyboard events ****/
    class KeyPressedEvent : public Event
    {
    public:
        int key_code;

        KeyPressedEvent(int key_code) : key_code(key_code) {}
        ~KeyPressedEvent() override {}
    };

    class KeyRelesedEvent : public Event
    {
    public:
        int key_code;

        KeyRelesedEvent(int key_code) : key_code(key_code) {}
    };

    /**** Input: mouse events ****/
    class MouseMovedEvent : public Event
    {
    public:
        float xpos;
        float ypos;

        MouseMovedEvent(float xpos, float ypos) : xpos(xpos), ypos(ypos) {}
    };

    class MouseButtonPressedEvent : public Event
    {
    public:
        int button;

        MouseButtonPressedEvent(int button) : button(button) {}
    };

    class MouseWheelScrollEvent : public Event
    {
    public:
        float scroll;

        MouseWheelScrollEvent(float scroll) : scroll(scroll) {}
    };
    
}