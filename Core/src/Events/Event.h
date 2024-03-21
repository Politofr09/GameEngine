#include <functional>
#include <vector>

#pragma once

namespace Core::Events
{
//
//	enum class EventType 
//	{
//		None = 0,
//		WindowCloose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
//		KeyPressed, KeyReleased,
//		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
//	};
//
//	enum EventCategory 
//	{
//		None = 0,
//		EventCategoryWindow,
//		EventCategoryInput,
//		EventCategoryMouse,
//		EventCategoryKeyboard
//	};
//
//#define EVENT_CLASS_TYPE(type) static EventType GetType() { return EventType::##type; }
//
//#define EVENT_CLASS_CATEGORY(category) static int GetCategory() const override { return category };

	class Event
	{
	public:
		bool Handled = false;

		virtual ~Event() = default;

		//virtual EventType GetEventType() = 0;

		//virtual int GetCategory() = 0;

		//inline bool IsInCategory(EventCategory category)
		//{
		//	return GetCategory() & category;
		//}
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