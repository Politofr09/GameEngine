#include "Events/Event.h"
#include "Events/InputEvents.h"
#include "Events/WindowEvents.h"

using namespace Core::Events;

// Declaration of static member variable
template <typename EventType>
std::vector<typename Dispatcher<EventType>::EventCallbackFn> Dispatcher<EventType>::_callbacks;