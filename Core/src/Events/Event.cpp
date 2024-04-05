#include "Event.h"

using namespace Core::Events;

std::unordered_map<Event::DescriptorType, std::vector<Dispatcher::EventCallbackFn>> Dispatcher::_observers;
