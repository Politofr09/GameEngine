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

        DECLARE_EVENT_TYPE("KeyPressedEvent");
    };

    class KeyDownEvent : public Event
    {
    public:
        int key_code;

        KeyDownEvent(int key_code) : key_code(key_code) {}
        ~KeyDownEvent() override {}

        DECLARE_EVENT_TYPE("KeyDownEvent");
    };

    class KeyReleasedEvent : public Event
    {
    public:
        int key_code;

        KeyReleasedEvent(int key_code) : key_code(key_code) {}

        DECLARE_EVENT_TYPE("KeyReleasedEvent");
    };

    /**** Input: mouse events ****/
    class MouseMovedEvent : public Event
    {
    public:
        float xpos;
        float ypos;

        MouseMovedEvent(float xpos, float ypos) : xpos(xpos), ypos(ypos) {}

        DECLARE_EVENT_TYPE("MouseMovedEvent");
    };

    class MouseButtonPressedEvent : public Event
    {
    public:
        int button;

        MouseButtonPressedEvent(int button) : button(button) {}

        DECLARE_EVENT_TYPE("MouseButtonPressedEvent");
    };

    class MouseScrollEvent : public Event
    {
    public:
        float scroll;

        MouseScrollEvent(float scroll) : scroll(scroll) {}
    
        DECLARE_EVENT_TYPE("MouseScrollEvent");
    };

}