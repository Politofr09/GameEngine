#include "Event.h"

#pragma once

namespace Core::Events
{
	
	/**** Window events ****/
	class WindowMovedEvent : public Event
	{
	public:
		int xpos, ypos;

		WindowMovedEvent(int xpos, int ypos) : xpos(xpos), ypos(ypos) {}

		DECLARE_EVENT_TYPE("WindowMovedEvent");
	};

	class WindowResizedEvent : public Event
	{
	public:
		int width, height;

		WindowResizedEvent(int width, int height) : width(width), height(height) {}

		DECLARE_EVENT_TYPE("WindowResizedEvent");
	};

	class WindowClosedEvent : public Event
	{
	public:
		~WindowClosedEvent() override
		{
		}

		DECLARE_EVENT_TYPE("WindowClosedEvent");
	};

}
