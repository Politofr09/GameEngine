#include "Event.h"

using namespace Core::Events;

std::vector<Dispatcher::EventCallbackFn> Dispatcher::_observers;
std::queue<Event*> Dispatcher::_eventQueue;

void Dispatcher::ProcessEvents()
{
	while (!_eventQueue.empty())
	{
		auto event = _eventQueue.front();
		_eventQueue.pop();

		for (const auto& observer : _observers)
		{
			if (event->Handled) continue;
			observer(event);
		}
	}
}
