#include "Event.h"

using namespace Core::Events;

std::vector<Dispatcher::EventCallbackFn> Dispatcher::_observers;
std::vector<Event*> Dispatcher::_eventQueue;
