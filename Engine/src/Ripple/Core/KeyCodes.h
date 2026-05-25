#pragma once

namespace Ripple
{
	typedef enum class KeyCode : uint16_t
	{
		// From glfw3.h
		Space = 32,
		Apostrophe = 39, /* ' */
		Comma = 44, /* , */
		Minus = 45, /* - */
		Period = 46, /* . */
		Slash = 47, /* / */

		D0 = 48, /* 0 */
		D1 = 49, /* 1 */
		D2 = 50, /* 2 */
		D3 = 51, /* 3 */
		D4 = 52, /* 4 */
		D5 = 53, /* 5 */
		D6 = 54, /* 6 */
		D7 = 55, /* 7 */
		D8 = 56, /* 8 */
		D9 = 57, /* 9 */

		Semicolon = 59, /* ; */
		Equal = 61, /* = */

		A = 65,
		B = 66,
		C = 67,
		D = 68,
		E = 69,
		F = 70,
		G = 71,
		H = 72,
		I = 73,
		J = 74,
		K = 75,
		L = 76,
		M = 77,
		N = 78,
		O = 79,
		P = 80,
		Q = 81,
		R = 82,
		S = 83,
		T = 84,
		U = 85,
		V = 86,
		W = 87,
		X = 88,
		Y = 89,
		Z = 90,

		LeftBracket = 91,  /* [ */
		Backslash = 92,  /* \ */
		RightBracket = 93,  /* ] */
		GraveAccent = 96,  /* ` */

		World1 = 161, /* non-US #1 */
		World2 = 162, /* non-US #2 */

		/* Function keys */
		Escape = 256,
		Enter = 257,
		Tab = 258,
		Backspace = 259,
		Insert = 260,
		Delete = 261,
		Right = 262,
		Left = 263,
		Down = 264,
		Up = 265,
		PageUp = 266,
		PageDown = 267,
		Home = 268,
		End = 269,
		CapsLock = 280,
		ScrollLock = 281,
		NumLock = 282,
		PrintScreen = 283,
		Pause = 284,
		F1 = 290,
		F2 = 291,
		F3 = 292,
		F4 = 293,
		F5 = 294,
		F6 = 295,
		F7 = 296,
		F8 = 297,
		F9 = 298,
		F10 = 299,
		F11 = 300,
		F12 = 301,
		F13 = 302,
		F14 = 303,
		F15 = 304,
		F16 = 305,
		F17 = 306,
		F18 = 307,
		F19 = 308,
		F20 = 309,
		F21 = 310,
		F22 = 311,
		F23 = 312,
		F24 = 313,
		F25 = 314,

		/* Keypad */
		KP0 = 320,
		KP1 = 321,
		KP2 = 322,
		KP3 = 323,
		KP4 = 324,
		KP5 = 325,
		KP6 = 326,
		KP7 = 327,
		KP8 = 328,
		KP9 = 329,
		KPDecimal = 330,
		KPDivide = 331,
		KPMultiply = 332,
		KPSubtract = 333,
		KPAdd = 334,
		KPEnter = 335,
		KPEqual = 336,

		LeftShift = 340,
		LeftControl = 341,
		LeftAlt = 342,
		LeftSuper = 343,
		RightShift = 344,
		RightControl = 345,
		RightAlt = 346,
		RightSuper = 347,
		Menu = 348
	} Key;

	inline std::ostream& operator<<(std::ostream& os, KeyCode keyCode)
	{
		os << static_cast<int32_t>(keyCode);
		return os;
	}
}

// From glfw3.h
#define RP_KEY_SPACE           ::Ripple::Key::Space
#define RP_KEY_APOSTROPHE      ::Ripple::Key::Apostrophe    /* ' */
#define RP_KEY_COMMA           ::Ripple::Key::Comma         /* , */
#define RP_KEY_MINUS           ::Ripple::Key::Minus         /* - */
#define RP_KEY_PERIOD          ::Ripple::Key::Period        /* . */
#define RP_KEY_SLASH           ::Ripple::Key::Slash         /* / */
#define RP_KEY_0               ::Ripple::Key::D0
#define RP_KEY_1               ::Ripple::Key::D1
#define RP_KEY_2               ::Ripple::Key::D2
#define RP_KEY_3               ::Ripple::Key::D3
#define RP_KEY_4               ::Ripple::Key::D4
#define RP_KEY_5               ::Ripple::Key::D5
#define RP_KEY_6               ::Ripple::Key::D6
#define RP_KEY_7               ::Ripple::Key::D7
#define RP_KEY_8               ::Ripple::Key::D8
#define RP_KEY_9               ::Ripple::Key::D9
#define RP_KEY_SEMICOLON       ::Ripple::Key::Semicolon     /* ; */
#define RP_KEY_EQUAL           ::Ripple::Key::Equal         /* = */
#define RP_KEY_A               ::Ripple::Key::A
#define RP_KEY_B               ::Ripple::Key::B
#define RP_KEY_C               ::Ripple::Key::C
#define RP_KEY_D               ::Ripple::Key::D
#define RP_KEY_E               ::Ripple::Key::E
#define RP_KEY_F               ::Ripple::Key::F
#define RP_KEY_G               ::Ripple::Key::G
#define RP_KEY_H               ::Ripple::Key::H
#define RP_KEY_I               ::Ripple::Key::I
#define RP_KEY_J               ::Ripple::Key::J
#define RP_KEY_K               ::Ripple::Key::K
#define RP_KEY_L               ::Ripple::Key::L
#define RP_KEY_M               ::Ripple::Key::M
#define RP_KEY_N               ::Ripple::Key::N
#define RP_KEY_O               ::Ripple::Key::O
#define RP_KEY_P               ::Ripple::Key::P
#define RP_KEY_Q               ::Ripple::Key::Q
#define RP_KEY_R               ::Ripple::Key::R
#define RP_KEY_S               ::Ripple::Key::S
#define RP_KEY_T               ::Ripple::Key::T
#define RP_KEY_U               ::Ripple::Key::U
#define RP_KEY_V               ::Ripple::Key::V
#define RP_KEY_W               ::Ripple::Key::W
#define RP_KEY_X               ::Ripple::Key::X
#define RP_KEY_Y               ::Ripple::Key::Y
#define RP_KEY_Z               ::Ripple::Key::Z
#define RP_KEY_LEFT_BRACKET    ::Ripple::Key::LeftBracket   /* [ */
#define RP_KEY_BACKSLASH       ::Ripple::Key::Backslash     /* \ */
#define RP_KEY_RIGHT_BRACKET   ::Ripple::Key::RightBracket  /* ] */
#define RP_KEY_GRAVE_ACCENT    ::Ripple::Key::GraveAccent   /* ` */
#define RP_KEY_WORLD_1         ::Ripple::Key::World1        /* non-US #1 */
#define RP_KEY_WORLD_2         ::Ripple::Key::World2        /* non-US #2 */

/* Function keys */
#define RP_KEY_ESCAPE          ::Ripple::Key::Escape
#define RP_KEY_ENTER           ::Ripple::Key::Enter
#define RP_KEY_TAB             ::Ripple::Key::Tab
#define RP_KEY_BACKSPACE       ::Ripple::Key::Backspace
#define RP_KEY_INSERT          ::Ripple::Key::Insert
#define RP_KEY_DELETE          ::Ripple::Key::Delete
#define RP_KEY_RIGHT           ::Ripple::Key::Right
#define RP_KEY_LEFT            ::Ripple::Key::Left
#define RP_KEY_DOWN            ::Ripple::Key::Down
#define RP_KEY_UP              ::Ripple::Key::Up
#define RP_KEY_PAGE_UP         ::Ripple::Key::PageUp
#define RP_KEY_PAGE_DOWN       ::Ripple::Key::PageDown
#define RP_KEY_HOME            ::Ripple::Key::Home
#define RP_KEY_END             ::Ripple::Key::End
#define RP_KEY_CAPS_LOCK       ::Ripple::Key::CapsLock
#define RP_KEY_SCROLL_LOCK     ::Ripple::Key::ScrollLock
#define RP_KEY_NUM_LOCK        ::Ripple::Key::NumLock
#define RP_KEY_PRINT_SCREEN    ::Ripple::Key::PrintScreen
#define RP_KEY_PAUSE           ::Ripple::Key::Pause
#define RP_KEY_F1              ::Ripple::Key::F1
#define RP_KEY_F2              ::Ripple::Key::F2
#define RP_KEY_F3              ::Ripple::Key::F3
#define RP_KEY_F4              ::Ripple::Key::F4
#define RP_KEY_F5              ::Ripple::Key::F5
#define RP_KEY_F6              ::Ripple::Key::F6
#define RP_KEY_F7              ::Ripple::Key::F7
#define RP_KEY_F8              ::Ripple::Key::F8
#define RP_KEY_F9              ::Ripple::Key::F9
#define RP_KEY_F10             ::Ripple::Key::F10
#define RP_KEY_F11             ::Ripple::Key::F11
#define RP_KEY_F12             ::Ripple::Key::F12
#define RP_KEY_F13             ::Ripple::Key::F13
#define RP_KEY_F14             ::Ripple::Key::F14
#define RP_KEY_F15             ::Ripple::Key::F15
#define RP_KEY_F16             ::Ripple::Key::F16
#define RP_KEY_F17             ::Ripple::Key::F17
#define RP_KEY_F18             ::Ripple::Key::F18
#define RP_KEY_F19             ::Ripple::Key::F19
#define RP_KEY_F20             ::Ripple::Key::F20
#define RP_KEY_F21             ::Ripple::Key::F21
#define RP_KEY_F22             ::Ripple::Key::F22
#define RP_KEY_F23             ::Ripple::Key::F23
#define RP_KEY_F24             ::Ripple::Key::F24
#define RP_KEY_F25             ::Ripple::Key::F25

/* Keypad */
#define RP_KEY_KP_0            ::Ripple::Key::KP0
#define RP_KEY_KP_1            ::Ripple::Key::KP1
#define RP_KEY_KP_2            ::Ripple::Key::KP2
#define RP_KEY_KP_3            ::Ripple::Key::KP3
#define RP_KEY_KP_4            ::Ripple::Key::KP4
#define RP_KEY_KP_5            ::Ripple::Key::KP5
#define RP_KEY_KP_6            ::Ripple::Key::KP6
#define RP_KEY_KP_7            ::Ripple::Key::KP7
#define RP_KEY_KP_8            ::Ripple::Key::KP8
#define RP_KEY_KP_9            ::Ripple::Key::KP9
#define RP_KEY_KP_DECIMAL      ::Ripple::Key::KPDecimal
#define RP_KEY_KP_DIVIDE       ::Ripple::Key::KPDivide
#define RP_KEY_KP_MULTIPLY     ::Ripple::Key::KPMultiply
#define RP_KEY_KP_SUBTRACT     ::Ripple::Key::KPSubtract
#define RP_KEY_KP_ADD          ::Ripple::Key::KPAdd
#define RP_KEY_KP_ENTER        ::Ripple::Key::KPEnter
#define RP_KEY_KP_EQUAL        ::Ripple::Key::KPEqual

#define RP_KEY_LEFT_SHIFT      ::Ripple::Key::LeftShift
#define RP_KEY_LEFT_CONTROL    ::Ripple::Key::LeftControl
#define RP_KEY_LEFT_ALT        ::Ripple::Key::LeftAlt
#define RP_KEY_LEFT_SUPER      ::Ripple::Key::LeftSuper
#define RP_KEY_RIGHT_SHIFT     ::Ripple::Key::RightShift
#define RP_KEY_RIGHT_CONTROL   ::Ripple::Key::RightControl
#define RP_KEY_RIGHT_ALT       ::Ripple::Key::RightAlt
#define RP_KEY_RIGHT_SUPER     ::Ripple::Key::RightSuper
#define RP_KEY_MENU            ::Ripple::Key::Menu

// Mouse (TODO: move into separate file probably)
//#define RP_MOUSE_BUTTON_LEFT    0
//#define RP_MOUSE_BUTTON_RIGHT   1
//#define RP_MOUSE_BUTTON_MIDDLE  2
