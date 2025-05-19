#pragma once
#include <cstdint>

namespace Spyen
{
	using KeyCode = uint32_t;
	
	enum : KeyCode {
		SPK_SPACE = 32,
		SPK_APOSTROPHE = 39,  /* ' */
		SPK_COMMA = 44, /* , */
		SPK_MINUS = 45,  /* - */
		SPK_PERIOD = 46, /* . */
		SPK_SLASH = 47,  /* / */
		SPK_0 = 48,
		SPK_1 = 49,
		SPK_2 = 50,
		SPK_3 = 51,
		SPK_4 = 52,
		SPK_5 = 53,
		SPK_6 = 54,
		SPK_7 = 55,
		SPK_8 = 56,
		SPK_9 = 57,
		SPK_SEMICOLON = 59, /* ; */
		SPK_EQUAL = 61, /* = */
		SPK_A = 65,
		SPK_B = 66,
		SPK_C = 67,
		SPK_D = 68,
		SPK_E = 69,
		SPK_F = 70,
		SPK_G = 71,
		SPK_H = 72,
		SPK_I = 73,
		SPK_J = 74,
		SPK_K = 75,
		SPK_L = 76,
		SPK_M = 77,
		SPK_N = 78,
		SPK_O = 79,
		SPK_P = 80,
		SPK_Q = 81,
		SPK_R = 82,
		SPK_S = 83,
		SPK_T = 84,
		SPK_U = 85,
		SPK_V = 86,
		SPK_W = 87,
		SPK_X = 88,
		SPK_Y = 89,
		SPK_Z = 90,
		SPK_LEFT_BRACKET = 91, /* [ */
		SPK_BACKSLASH = 92, /* \ */
		SPK_RIGHT_BRACKET = 93, /* ] */
		SPK_GRAVE_ACCENT = 96, /* ` */
		SPK_WORLD_1 = 161, /* non-US #1 */
		SPK_WORLD_2 = 162,/* non-US #2 */

		/* Function keys */
		SPK_ESCAPE = 256,
		SPK_ENTER = 257,
		SPK_TAB = 258,
		SPK_BACKSPACE = 259,
		SPK_INSERT = 260,
		SPK_DELETE = 261,
		SPK_RIGHT = 262,
		SPK_LEFT = 263,
		SPK_DOWN = 264,
		SPK_UP = 265,
		SPK_PAGE_UP = 266,
		SPK_PAGE_DOWN = 267,
		SPK_HOME = 268,
		SPK_END = 269,
		SPK_CAPS_LOCK = 280,
		SPK_SCROLL_LOCK = 281,
		SPK_NUM_LOCK = 282,
		SPK_PRINT_SCREEN = 283,
		SPK_PAUSE = 284,
		SPK_F1 = 290,
		SPK_F2 = 291,
		SPK_F3 = 292,
		SPK_F4 = 293,
		SPK_F5 = 294,
		SPK_F6 = 295,
		SPK_F7 = 296,
		SPK_F8 = 297,
		SPK_F9 = 298,
		SPK_F10 = 299,
		SPK_F11 = 300,
		SPK_F12 = 301,
		SPK_F13 = 302,
		SPK_F14 = 303,
		SPK_F15 = 304,
		SPK_F16 = 305,
		SPK_F17 = 306,
		SPK_F18 = 307,
		SPK_F19 = 308,
		SPK_F20 = 309,
		SPK_F21 = 310,
		SPK_F22 = 311,
		SPK_F23 = 312,
		SPK_F24 = 313,
		SPK_F25 = 314,
		SPK_KP_0 = 320,
		SPK_KP_1 = 321,
		SPK_KP_2 = 322,
		SPK_KP_3 = 323,
		SPK_KP_4 = 324,
		SPK_KP_5 = 325,
		SPK_KP_6 = 326,
		SPK_KP_7 = 327,
		SPK_KP_8 = 328,
		SPK_KP_9 = 329,
		SPK_KP_DECIMAL = 330,
		SPK_KP_DIVIDE = 331,
		SPK_KP_MULTIPLY = 332,
		SPK_KP_SUBTRACT = 333,
		SPK_KP_ADD = 334,
		SPK_KP_ENTER = 335,
		SPK_KP_EQUAL = 336,
		SPK_SHIFT = 340,
		SPK_CTRL = 341,
		SPK_ALT = 342,
		SPK_SUPER = 343,
		SPK_RIGHT_SHIFT = 344,
		SPK_RIGHT_CONTROL = 345,
		SPK_RIGHT_ALT = 346,
		SPK_RIGHT_SUPER = 347,
		SPK_MENU = 348,

	};
}
