#include "Event.h"

#pragma once

namespace Core::Events
{
	
	class WindowClosedEvent : public Event
	{
	public:

		bool WindowClosed = true;

		~WindowClosedEvent() override
		{
		}
	};

}
