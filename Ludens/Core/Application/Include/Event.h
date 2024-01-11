#pragma once

#include "Core/Application/Include/Input.h"

namespace LD {

	struct Event;
	typedef bool (*EventHandlerFn)(const Event& event);

	bool EventDispatch(Event& event, EventHandlerFn handler);

	enum class EventType
	{
		None = 0,
		ApplicationQuit,
		KeyPressed,
		KeyReleased,
		MouseMotion,
		MouseScrolled,
		MouseButtonPressed,
		MouseButtonReleased,
	};

	using EventFlags = unsigned char;
	enum EventFlagBits
	{
		EVENT_FLAGS_NONE = 0,
		EVENT_FLAGS_APPLICATION_BIT = 1<<0,
		EVENT_FLAGS_INPUT_BIT = 1<<1,
		EVENT_FLAGS_INPUT_KEY_BIT = 1<<2,
		EVENT_FLAGS_INPUT_MOUSE_BIT = 1<<3,
	};

	struct Event
	{
		Event() = delete;
		Event(EventType type, EventFlags flags)
			: Type(type), Flags(flags), IsHandled(false) {}
		virtual ~Event() {}

		inline bool IsApplicationEvent() const { return (Flags & EVENT_FLAGS_APPLICATION_BIT) == EVENT_FLAGS_APPLICATION_BIT; }
		inline bool IsInputEvent() const { return (Flags & EVENT_FLAGS_INPUT_BIT) == EVENT_FLAGS_INPUT_BIT; }

		EventType Type = EventType::None;
		EventFlags Flags = EVENT_FLAGS_NONE;
		bool IsHandled;
	};

	struct ApplicationQuitEvent : Event
	{
		ApplicationQuitEvent()
			: Event(EventType::ApplicationQuit, EVENT_FLAGS_APPLICATION_BIT) {};
		~ApplicationQuitEvent() {}
	};

	struct KeyPressedEvent : Event
	{
		KeyPressedEvent()
			: Event(EventType::KeyPressed, EVENT_FLAGS_INPUT_BIT | EVENT_FLAGS_INPUT_KEY_BIT) {};
		~KeyPressedEvent() {}

		KeyCode Key;
		bool Repeat;
	};

	struct KeyReleasedEvent : Event
	{
		KeyReleasedEvent()
			: Event(EventType::KeyReleased, EVENT_FLAGS_INPUT_BIT | EVENT_FLAGS_INPUT_KEY_BIT) {};
		~KeyReleasedEvent() {}

		KeyCode Key;
	};

	struct MouseMotionEvent : Event
	{
		MouseMotionEvent()
			: Event(EventType::MouseMotion, EVENT_FLAGS_INPUT_BIT | EVENT_FLAGS_INPUT_MOUSE_BIT) {}
		~MouseMotionEvent() {}

		float XPos;
		float YPos;
	};

	struct MouseScrolledEvent : Event
	{
		MouseScrolledEvent()
			: Event(EventType::MouseScrolled, EVENT_FLAGS_INPUT_BIT | EVENT_FLAGS_INPUT_MOUSE_BIT) {}
		~MouseScrolledEvent() {}

		float XOffset;
		float YOffset;
	};

	struct MouseButtonPressedEvent : Event
	{
		MouseButtonPressedEvent()
			: Event(EventType::MouseButtonPressed, EVENT_FLAGS_INPUT_BIT | EVENT_FLAGS_INPUT_MOUSE_BIT) {};
		~MouseButtonPressedEvent() {}

		MouseButton Button;
	};

	struct MouseButtonReleasedEvent : Event
	{
		MouseButtonReleasedEvent()
			: Event(EventType::MouseButtonReleased, EVENT_FLAGS_INPUT_BIT | EVENT_FLAGS_INPUT_MOUSE_BIT) {};
		~MouseButtonReleasedEvent() {}

		MouseButton Button;
	};

} // namespace LD