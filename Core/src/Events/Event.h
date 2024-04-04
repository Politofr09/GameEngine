#include <functional>
#include <vector>

#pragma once

namespace Core::Events
{

#define INSTANTIATE_DISPATCHER(EventType) \
    template class Dispatcher<EventType>;

	class Event
	{
	public:
		bool Handled = false;

		virtual ~Event() = default;

	};

	template<typename EventType>
	class Dispatcher
	{
	private:
		using EventCallbackFn = std::function<void(EventType&)>;
		static std::vector<EventCallbackFn> _callbacks;

	public:
		static void Trigger(Event& event)
		{
			for (auto& callback : _callbacks)
			{
				if (event.Handled) break;
				callback(*dynamic_cast<EventType*>(&event));
			}
		}

		static void Subscribe(EventCallbackFn callback) 
		{
			_callbacks.push_back(callback);
		}

		~Dispatcher()
		{
			_callbacks.clear();
		}
	};
	
}