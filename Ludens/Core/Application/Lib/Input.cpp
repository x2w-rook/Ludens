#pragma once

#include "Core/Application/Include/Input.h"
#include "Core/Application/Include/Application.h"
#include "Core/Application/Include/Event.h"
#include "Core/OS/Include/Error.h"


namespace LD {
namespace Input {

	bool sKey[KEY_CODE_ENUM_LAST];
	bool sKeyPressed[KEY_CODE_ENUM_LAST];
	bool sKeyReleased[KEY_CODE_ENUM_LAST];
	bool sMouseButton[MOUSE_BUTTON_ENUM_LAST];
	bool sMouseButtonPressed[MOUSE_BUTTON_ENUM_LAST];
	bool sMouseButtonReleased[MOUSE_BUTTON_ENUM_LAST];

	bool GetKey(KeyCode key)
	{
		return sKey[key];
	}

	bool GetKeyPressed(KeyCode key)
	{
		return sKeyPressed[key];
	}

	bool GetKeyReleased(KeyCode key)
	{
		return sKeyReleased[key];
	}

	bool GetMouseButton(MouseButton button)
	{
		return sMouseButton[button];
	}

	bool GetMouseButtonPressed(MouseButton button)
	{
		return sMouseButtonPressed[button];
	}

	bool GetMouseButtonReleased(MouseButton button)
	{
		return sMouseButtonReleased[button];
	}

} // namespace Input

	void Application::OnInputEvent(const Event& inputEvent)
	{
		LD_DEBUG_ASSERT(inputEvent.IsInputEvent());

		switch (inputEvent.Type)
		{
		case EventType::KeyPressed:
		{
			const KeyPressedEvent& event = static_cast<const KeyPressedEvent&>(inputEvent);
			Input::sKey[event.Key] = true;
			if (!event.Repeat)
				Input::sKeyPressed[event.Key] = true;
			break;
		}
		case EventType::KeyReleased:
		{
			const KeyReleasedEvent& event = static_cast<const KeyReleasedEvent&>(inputEvent);
			Input::sKey[event.Key] = false;
			Input::sKeyReleased[event.Key] = true;
			break;
		}
		case EventType::MouseButtonPressed:
		{
			const MouseButtonPressedEvent& event = static_cast<const MouseButtonPressedEvent&>(inputEvent);
			Input::sMouseButton[event.Button] = true;
			Input::sMouseButtonPressed[event.Button] = true;
			break;
		}
		case EventType::MouseButtonReleased:
		{
			const MouseButtonReleasedEvent& event = static_cast<const MouseButtonReleasedEvent&>(inputEvent);
			Input::sMouseButton[event.Button] = false;
			Input::sMouseButtonReleased[event.Button] = true;
			break;
		}
		default:
			break;
		}
	}

	void Application::OnInputNewFrame()
	{
		for (size_t i = 0; i < KEY_CODE_ENUM_LAST; i++)
		{
			Input::sKeyPressed[i] = false;
			Input::sKeyReleased[i] = false;
		}

		for (size_t i = 0; i < MOUSE_BUTTON_ENUM_LAST; i++)
		{
			Input::sMouseButtonPressed[i] = false;
			Input::sMouseButtonReleased[i] = false;
		}
	}

} // namespace LD