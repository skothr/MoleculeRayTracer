#ifndef APOLLO_KEYBOARD_H
#define APOLLO_KEYBOARD_H

#include <Windows.h>
#include "AStatus.h"

enum class KeyCode
{
	K_UNKNOWN = -1,

	K_SPACE = 0,
	K_RETURN,
	K_BACKSPACE,
	K_TAB,
	K_ESCAPE,

	K_PAGEUP,
	K_PAGEDOWN,
	K_END,
	K_HOME,
	K_INSERT,
	K_DELETE,
	K_ADD,
	K_SUBTRACT,
	K_MULTIPLY,
	K_DIVIDE,

	K_MEDIA_PLAY_PAUSE,
	K_MEDIA_STOP,
	K_MEDIA_NEXT,
	K_MEDIA_PREV,

	K_VOLUME_UP,
	K_VOLUME_DOWN,
	K_VOLUME_MUTE,

	K_F1,
	K_F2,
	K_F3,
	K_F4,
	K_F5,
	K_F6,
	K_F7,
	K_F8,
	K_F9,
	K_F10,
	K_F11,
	K_F12,
	K_F13,
	K_F14,
	K_F15,
	K_F16,
	K_F17,
	K_F18,
	K_F19,
	K_F20,
	K_F21,
	K_F22,
	K_F23,
	K_F24,

	K_LEFTARROW,
	K_RIGHTARROW,
	K_UPARROW,
	K_DOWNARROW,

	K_LSHIFT,
	K_RSHIFT,
	K_LCTRL,
	K_RCTRL,
	K_LALT,
	K_RALT,

	K_LSYSTEM,
	K_RSYSTEM,

	K_SEMICOLON,
	K_FSLASH,
	K_BSLASH,
	K_EQUAL,
	K_DASH,
	K_LBRACKET,
	K_RBRACKET,
	K_COMMA,
	K_PERIOD,
	K_QUOTE,
	K_TILDE,

	K_A,
	K_B,
	K_C,
	K_D,
	K_E,
	K_F,
	K_G,
	K_H,
	K_I,
	K_J,
	K_K,
	K_L,
	K_M,
	K_N,
	K_O,
	K_P,
	K_Q,
	K_R,
	K_S,
	K_T,
	K_U,
	K_V,
	K_W,
	K_X,
	K_Y,
	K_Z,

	K_0,
	K_1,
	K_2,
	K_3,
	K_4,
	K_5,
	K_6,
	K_7,
	K_8,
	K_9,
	
	K_NUMPAD0,
	K_NUMPAD1,
	K_NUMPAD2,
	K_NUMPAD3,
	K_NUMPAD4,
	K_NUMPAD5,
	K_NUMPAD6,
	K_NUMPAD7,
	K_NUMPAD8,
	K_NUMPAD9,

	K_COUNT
};
typedef KeyCode Keys;

class Keyboard
{
private:
	static bool keyStates[static_cast<unsigned int>(Keys::K_COUNT)];
	Keyboard() { }

public:
	static bool keyDown(KeyCode key);
	static AStatus updateStates();
};


inline KeyCode convertWindowsKeyCode(WPARAM winKey, LPARAM flags)
{
	switch(winKey)
	{

	//Check flags to differentiate between left/right buttons
	case VK_SHIFT:
	{
		static UINT lShift = MapVirtualKey(VK_LSHIFT, MAPVK_VK_TO_VSC);
		UINT scancode = static_cast<UINT>((flags & (0xFF << 16)) >> 16);
		return (scancode == lShift ? Keys::K_LSHIFT : Keys::K_RSHIFT);
	}
	case VK_MENU:
		return (HIWORD(flags) & KF_EXTENDED) ? Keys::K_RALT : Keys::K_LALT;
	case VK_CONTROL:
		return (HIWORD(flags) & KF_EXTENDED) ? Keys::K_RCTRL : Keys::K_LCTRL;


	case VK_SPACE:				return Keys::K_SPACE;
	case VK_RETURN:				return Keys::K_RETURN;
	case VK_BACK:				return Keys::K_BACKSPACE;
	case VK_TAB:				return Keys::K_TAB;
	case VK_ESCAPE:				return Keys::K_ESCAPE;

	case VK_PRIOR:				return Keys::K_PAGEUP;
	case VK_NEXT:				return Keys::K_PAGEDOWN;
	case VK_END:				return Keys::K_END;
	case VK_HOME:				return Keys::K_HOME;
	case VK_INSERT:				return Keys::K_INSERT;
	case VK_DELETE:				return Keys::K_DELETE;
	case VK_ADD:				return Keys::K_ADD;
	case VK_SUBTRACT:			return Keys::K_SUBTRACT;
	case VK_MULTIPLY:			return Keys::K_MULTIPLY;
	case VK_DIVIDE:				return Keys::K_DIVIDE;

	case VK_MEDIA_PLAY_PAUSE:	return Keys::K_MEDIA_PLAY_PAUSE;
	case VK_MEDIA_STOP:			return Keys::K_MEDIA_STOP;
	case VK_MEDIA_NEXT_TRACK:	return Keys::K_MEDIA_NEXT;
	case VK_MEDIA_PREV_TRACK:	return Keys::K_MEDIA_PREV;
		
	//case VK_VOLUME_UP:			return Keys::K_VOLUME_UP;
	//case VK_VOLUME_DOWN:		return Keys::K_VOLUME_DOWN;
	//case VK_VOLUME_MUTE:		return Keys::K_VOLUME_MUTE;

	case VK_F1:					return Keys::K_F1;
	case VK_F2:					return Keys::K_F2;
	case VK_F3:					return Keys::K_F3;
	case VK_F4:					return Keys::K_F4;
	case VK_F5:					return Keys::K_F5;
	case VK_F6:					return Keys::K_F6;
	case VK_F7:					return Keys::K_F7;
	case VK_F8:					return Keys::K_F8;
	case VK_F9:					return Keys::K_F9;
	case VK_F10:				return Keys::K_F10;
	case VK_F11:				return Keys::K_F11;
	case VK_F12:				return Keys::K_F12;
	case VK_F13:				return Keys::K_F13;
	case VK_F14:				return Keys::K_F14;
	case VK_F15:				return Keys::K_F15;
	case VK_F16:				return Keys::K_F16;
	case VK_F17:				return Keys::K_F17;
	case VK_F18:				return Keys::K_F18;
	case VK_F19:				return Keys::K_F19;
	case VK_F20:				return Keys::K_F20;
	case VK_F21:				return Keys::K_F21;
	case VK_F22:				return Keys::K_F22;
	case VK_F23:				return Keys::K_F23;
	case VK_F24:				return Keys::K_F24;

	case VK_LEFT:				return Keys::K_LEFTARROW;
	case VK_RIGHT:				return Keys::K_RIGHTARROW;
	case VK_UP:					return Keys::K_UPARROW;
	case VK_DOWN:				return Keys::K_DOWNARROW;

	case VK_LWIN:				return Keys::K_LSYSTEM;
	case VK_RWIN:				return Keys::K_RSYSTEM;

	case VK_OEM_1:				return Keys::K_SEMICOLON;
	case VK_OEM_2:				return Keys::K_FSLASH;
	case VK_OEM_5:				return Keys::K_BSLASH;
	case VK_OEM_PLUS:			return Keys::K_EQUAL;
	case VK_OEM_MINUS:			return Keys::K_DASH;
	case VK_OEM_4:				return Keys::K_LBRACKET;
	case VK_OEM_6:				return Keys::K_RBRACKET;
	case VK_OEM_COMMA:			return Keys::K_COMMA;
	case VK_OEM_PERIOD:			return Keys::K_PERIOD;
	case VK_OEM_7:				return Keys::K_QUOTE;
	case VK_OEM_3:				return Keys::K_TILDE;

	case 'A':					return Keys::K_A;
	case 'B':					return Keys::K_B;
	case 'C':					return Keys::K_C;
	case 'D':					return Keys::K_D;
	case 'E':					return Keys::K_E;
	case 'F':					return Keys::K_F;
	case 'G':					return Keys::K_G;
	case 'H':					return Keys::K_H;
	case 'I':					return Keys::K_I;
	case 'J':					return Keys::K_J;
	case 'K':					return Keys::K_K;
	case 'L':					return Keys::K_L;
	case 'M':					return Keys::K_M;
	case 'N':					return Keys::K_N;
	case 'O':					return Keys::K_O;
	case 'P':					return Keys::K_P;
	case 'Q':					return Keys::K_Q;
	case 'R':					return Keys::K_R;
	case 'S':					return Keys::K_S;
	case 'T':					return Keys::K_T;
	case 'U':					return Keys::K_U;
	case 'V':					return Keys::K_V;
	case 'W':					return Keys::K_W;
	case 'X':					return Keys::K_X;
	case 'Y':					return Keys::K_Y;
	case 'Z':					return Keys::K_Z;

	case '0':					return Keys::K_0;
	case '1':					return Keys::K_1;
	case '2':					return Keys::K_2;
	case '3':					return Keys::K_3;
	case '4':					return Keys::K_4;
	case '5':					return Keys::K_5;
	case '6':					return Keys::K_6;
	case '7':					return Keys::K_7;
	case '8':					return Keys::K_8;
	case '9':					return Keys::K_9;
	
	case VK_NUMPAD0:			return Keys::K_NUMPAD0;
	case VK_NUMPAD1:			return Keys::K_NUMPAD1;
	case VK_NUMPAD2:			return Keys::K_NUMPAD2;
	case VK_NUMPAD3:			return Keys::K_NUMPAD3;
	case VK_NUMPAD4:			return Keys::K_NUMPAD4;
	case VK_NUMPAD5:			return Keys::K_NUMPAD5;
	case VK_NUMPAD6:			return Keys::K_NUMPAD6;
	case VK_NUMPAD7:			return Keys::K_NUMPAD7;
	case VK_NUMPAD8:			return Keys::K_NUMPAD8;
	case VK_NUMPAD9:			return Keys::K_NUMPAD9;

	default:					return Keys::K_UNKNOWN;
	}
}


#endif	//APOLLO_KEYBOARD_H