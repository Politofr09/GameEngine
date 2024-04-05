#include <functional>
#include <unordered_map>
#include <vector>

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
		using EventCallbackFn = std::function<void(Event&)>;
		static std::unordered_map<Event::DescriptorType, std::vector<EventCallbackFn>> _observers;

	public:
		static void Trigger(Event& event)
		{
			auto type = event.GetType();
			
			if (_observers.find(type) == _observers.end())
				return;

			auto& observers = _observers.at(type);
			for (auto& observer : observers)
			{
				observer(event);
				if (event.Handled) 
					break;
			}
		}

		static void Subscribe(Event::DescriptorType eventType, EventCallbackFn callback)
		{
			_observers[eventType].push_back(callback);
		}

		~Dispatcher()
		{
			_observers.clear();
		}
	};
	
}