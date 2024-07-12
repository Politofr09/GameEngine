#include <functional>
#include <unordered_map>
#include <vector>
#include <iostream>

#pragma once

namespace Core::Events
{

#define DECLARE_EVENT_TYPE(event) static constexpr DescriptorType descriptor = event; \
								  virtual DescriptorType GetType() const override { return descriptor; }

	class Event
	{
	public:
		bool Handled = false;

		virtual ~Event() = default;

		using DescriptorType = const char*;

		virtual DescriptorType GetType() const = 0;
	};

	class Dispatcher
	{
	private:
		using EventCallbackFn = std::function<void(Event*)>;
		static std::vector<EventCallbackFn> _observers;
		static std::vector<Event*> _eventQueue;

	public:
		static void ProcessEvents()
		{
			while (!_eventQueue.empty())
			{
				auto event = _eventQueue.front();
				_eventQueue.erase(_eventQueue.begin());

				for (auto& observer : _observers)
				{
					observer(event);
					//if (event->Handled)
					//	break;
				}
			}
		}

		static void Trigger(Event* event)
		{
			_eventQueue.push_back(event);
		}

		// Subscribe to a specific event type
		static void Subscribe(EventCallbackFn callback)
		{
			_observers.push_back(callback);
		}

		~Dispatcher()
		{
			_observers.clear();
		}
	};
	
}