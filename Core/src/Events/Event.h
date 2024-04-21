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
		static std::vector<EventCallbackFn> _observers;

	public:
		static void Trigger(Event& event)
		{
			auto type = event.GetType();

			for (auto& observer : _observers)
			{
				observer(event);
				if (event.Handled) 
					return;
			}
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