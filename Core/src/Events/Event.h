#include <functional>
#include <unordered_map>
#include <queue>
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
		static std::queue<Event*> _eventQueue;

	public:
		static void ProcessEvents();

		static void Trigger(Event* event)
		{
			_eventQueue.push(event);
		}
		/// BIG WARNING: (Only when using std::bind)
		// - Make sure to use std::bind properly and make sure you don't call it on the same class twice! 
		//   (at least not with the same std::placeholder)
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