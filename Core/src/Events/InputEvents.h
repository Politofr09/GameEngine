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
    INSTANTIATE_DISPATCHER(KeyPressedEvent);

    class KeyReleasedEvent : public Event
    {
    public:
        int key_code;

        KeyReleasedEvent(int key_code) : key_code(key_code) {}
    };
    INSTANTIATE_DISPATCHER(KeyReleasedEvent);

    /**** Input: mouse events ****/
    class MouseMovedEvent : public Event
    {
    public:
        float xpos;
        float ypos;

        MouseMovedEvent(float xpos, float ypos) : xpos(xpos), ypos(ypos) {}
    };
    INSTANTIATE_DISPATCHER(MouseMovedEvent);

    class MouseButtonPressedEvent : public Event
    {
    public:
        int button;

        MouseButtonPressedEvent(int button) : button(button) {}
    };
    INSTANTIATE_DISPATCHER(MouseButtonPressedEvent);

    class MouseWheelScrollEvent : public Event
    {
    public:
        float scroll;

        MouseWheelScrollEvent(float scroll) : scroll(scroll) {}
    };
    INSTANTIATE_DISPATCHER(MouseWheelScrollEvent);

}