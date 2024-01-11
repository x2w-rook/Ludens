#pragma once


namespace LD {

	// Input enums are proudly stolen from GLFW

	enum KeyCode : int
	{
		KEY_CODE_SPACE             = 32,
		KEY_CODE_APOSTROPHE        = 39,  /* ' */
		KEY_CODE_COMMA             = 44,  /* , */
		KEY_CODE_MINUS             = 45,  /* - */
		KEY_CODE_PERIOD            = 46,  /* . */
		KEY_CODE_SLASH             = 47,  /* / */
		KEY_CODE_0                 = 48,
		KEY_CODE_1                 = 49,
		KEY_CODE_2                 = 50,
		KEY_CODE_3                 = 51,
		KEY_CODE_4                 = 52,
		KEY_CODE_5                 = 53,
		KEY_CODE_6                 = 54,
		KEY_CODE_7                 = 55,
		KEY_CODE_8                 = 56,
		KEY_CODE_9                 = 57,
		KEY_CODE_SEMICOLON         = 59,  /* ; */
		KEY_CODE_EQUAL             = 61,  /* = */
		KEY_CODE_A                 = 65,
		KEY_CODE_B                 = 66,
		KEY_CODE_C                 = 67,
		KEY_CODE_D                 = 68,
		KEY_CODE_E                 = 69,
		KEY_CODE_F                 = 70,
		KEY_CODE_G                 = 71,
		KEY_CODE_H                 = 72,
		KEY_CODE_I                 = 73,
		KEY_CODE_J                 = 74,
		KEY_CODE_K                 = 75,
		KEY_CODE_L                 = 76,
		KEY_CODE_M                 = 77,
		KEY_CODE_N                 = 78,
		KEY_CODE_O                 = 79,
		KEY_CODE_P                 = 80,
		KEY_CODE_Q                 = 81,
		KEY_CODE_R                 = 82,
		KEY_CODE_S                 = 83,
		KEY_CODE_T                 = 84,
		KEY_CODE_U                 = 85,
		KEY_CODE_V                 = 86,
		KEY_CODE_W                 = 87,
		KEY_CODE_X                 = 88,
		KEY_CODE_Y                 = 89,
		KEY_CODE_Z                 = 90,
		KEY_CODE_LEFT_BRACKET      = 91,  /* [ */
		KEY_CODE_BACKSLASH         = 92,  /* \ */
		KEY_CODE_RIGHT_BRACKET     = 93,  /* ] */
		KEY_CODE_GRAVE_ACCENT      = 96,  /* ` */
		KEY_CODE_WORLD_1           = 161, /* non-US #1 */
		KEY_CODE_WORLD_2           = 162, /* non-US #2 */

		// TODO: function keys
		KEY_CODE_ENUM_LAST = KEY_CODE_WORLD_2
	};

	enum MouseButton : int
	{
		MOUSE_BUTTON_0 = 0,
		MOUSE_BUTTON_1 = 1,
		MOUSE_BUTTON_2 = 2,
		MOUSE_BUTTON_3 = 3,
		MOUSE_BUTTON_4 = 4,
		MOUSE_BUTTON_5 = 5,
		MOUSE_BUTTON_6 = 6,
		MOUSE_BUTTON_7 = 7,
		MOUSE_BUTTON_ENUM_LAST = MOUSE_BUTTON_7,

		MOUSE_BUTTON_LEFT   = MOUSE_BUTTON_0,
		MOUSE_BUTTON_RIGHT  = MOUSE_BUTTON_1,
		MOUSE_BUTTON_MIDDLE = MOUSE_BUTTON_2,
	};

namespace Input {

	// Keyboard Input Polling
	bool GetKey(KeyCode key);
	bool GetKeyPressed(KeyCode key);
	bool GetKeyReleased(KeyCode key);

	// Mouse Input Polling
	bool GetMouseButton(MouseButton button);
	bool GetMouseButtonPressed(MouseButton button);
	bool GetMouseButtonReleased(MouseButton button);

} // namespace Input
} // namespace LD