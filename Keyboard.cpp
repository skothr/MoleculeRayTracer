#include "Keyboard.h"


/////KEYBOARD/////

bool Keyboard::keyStates[static_cast<unsigned int>(Keys::K_COUNT)] {false};

bool Keyboard::keyDown(KeyCode key)
{
	return keyStates[static_cast<unsigned int>(key)];
}

AStatus Keyboard::updateStates()
{
	AStatus status;
	BYTE *win_states = new BYTE[256];

	static const BYTE msb = 0x80;

	if(GetKeyboardState(win_states))
	{
		//Set keyStates based on windows key states

		keyStates[static_cast<unsigned int>(Keys::K_LSHIFT)]		= win_states[VK_LSHIFT] & msb;
		keyStates[static_cast<unsigned int>(Keys::K_RSHIFT)]		= win_states[VK_RSHIFT] & msb;
		keyStates[static_cast<unsigned int>(Keys::K_LALT)]			= win_states[VK_LMENU] & msb;
		keyStates[static_cast<unsigned int>(Keys::K_RALT)]			= win_states[VK_RMENU] & msb;
		keyStates[static_cast<unsigned int>(Keys::K_LCTRL)]			= win_states[VK_LCONTROL] & msb;
		keyStates[static_cast<unsigned int>(Keys::K_RCTRL)]			= win_states[VK_RCONTROL] & msb;

		keyStates[static_cast<unsigned int>(Keys::K_SPACE)]			= win_states[VK_SPACE] & msb;
		keyStates[static_cast<unsigned int>(Keys::K_RETURN)]		= win_states[VK_RETURN] & msb;
		keyStates[static_cast<unsigned int>(Keys::K_BACKSPACE)]		= win_states[VK_BACK] & msb;
		keyStates[static_cast<unsigned int>(Keys::K_TAB)]			= win_states[VK_TAB] & msb;
		keyStates[static_cast<unsigned int>(Keys::K_ESCAPE)]		= win_states[VK_ESCAPE] & msb;

		keyStates[static_cast<unsigned int>(Keys::K_PAGEUP)]		= win_states[VK_PRIOR] & msb;
		keyStates[static_cast<unsigned int>(Keys::K_PAGEDOWN)]		= win_states[VK_NEXT] & msb;
		keyStates[static_cast<unsigned int>(Keys::K_END)]			= win_states[VK_END] & msb;
		keyStates[static_cast<unsigned int>(Keys::K_HOME)]			= win_states[VK_HOME] & msb;
		keyStates[static_cast<unsigned int>(Keys::K_INSERT)]		= win_states[VK_INSERT] & msb;
		keyStates[static_cast<unsigned int>(Keys::K_DELETE)]		= win_states[VK_DELETE] & msb;
		keyStates[static_cast<unsigned int>(Keys::K_ADD)]			= win_states[VK_ADD] & msb;
		keyStates[static_cast<unsigned int>(Keys::K_SUBTRACT)]		= win_states[VK_SUBTRACT] & msb;
		keyStates[static_cast<unsigned int>(Keys::K_MULTIPLY)]		= win_states[VK_MULTIPLY] & msb;
		keyStates[static_cast<unsigned int>(Keys::K_DIVIDE)]		= win_states[VK_DIVIDE] & msb;

		keyStates[static_cast<unsigned int>(Keys::K_MEDIA_PLAY_PAUSE)]	= win_states[VK_MEDIA_PLAY_PAUSE] & msb;
		keyStates[static_cast<unsigned int>(Keys::K_MEDIA_STOP)]		= win_states[VK_MEDIA_STOP] & msb;
		keyStates[static_cast<unsigned int>(Keys::K_MEDIA_NEXT)]		= win_states[VK_MEDIA_NEXT_TRACK] & msb;
		keyStates[static_cast<unsigned int>(Keys::K_MEDIA_PREV)]		= win_states[VK_MEDIA_PREV_TRACK] & msb;

		keyStates[static_cast<unsigned int>(Keys::K_F1)]			= win_states[VK_F1] & msb;
		keyStates[static_cast<unsigned int>(Keys::K_F2)] 			= win_states[VK_F2] & msb;
		keyStates[static_cast<unsigned int>(Keys::K_F3)] 			= win_states[VK_F3] & msb;
		keyStates[static_cast<unsigned int>(Keys::K_F4)] 			= win_states[VK_F4] & msb;
		keyStates[static_cast<unsigned int>(Keys::K_F5)] 			= win_states[VK_F5] & msb;
		keyStates[static_cast<unsigned int>(Keys::K_F6)] 			= win_states[VK_F6] & msb;
		keyStates[static_cast<unsigned int>(Keys::K_F7)] 			= win_states[VK_F7] & msb;
		keyStates[static_cast<unsigned int>(Keys::K_F8)] 			= win_states[VK_F8] & msb;
		keyStates[static_cast<unsigned int>(Keys::K_F9)] 			= win_states[VK_F9] & msb;
		keyStates[static_cast<unsigned int>(Keys::K_F10)] 			= win_states[VK_F10] & msb;
		keyStates[static_cast<unsigned int>(Keys::K_F11)] 			= win_states[VK_F11] & msb;
		keyStates[static_cast<unsigned int>(Keys::K_F12)] 			= win_states[VK_F12] & msb;
		keyStates[static_cast<unsigned int>(Keys::K_F13)] 			= win_states[VK_F13] & msb;
		keyStates[static_cast<unsigned int>(Keys::K_F14)] 			= win_states[VK_F14] & msb;
		keyStates[static_cast<unsigned int>(Keys::K_F15)] 			= win_states[VK_F15] & msb;
		keyStates[static_cast<unsigned int>(Keys::K_F16)] 			= win_states[VK_F16] & msb;
		keyStates[static_cast<unsigned int>(Keys::K_F17)] 			= win_states[VK_F17] & msb;
		keyStates[static_cast<unsigned int>(Keys::K_F18)] 			= win_states[VK_F18] & msb;
		keyStates[static_cast<unsigned int>(Keys::K_F19)] 			= win_states[VK_F19] & msb;
		keyStates[static_cast<unsigned int>(Keys::K_F20)] 			= win_states[VK_F20] & msb;
		keyStates[static_cast<unsigned int>(Keys::K_F21)] 			= win_states[VK_F21] & msb;
		keyStates[static_cast<unsigned int>(Keys::K_F22)] 			= win_states[VK_F22] & msb;
		keyStates[static_cast<unsigned int>(Keys::K_F23)] 			= win_states[VK_F23] & msb;
		keyStates[static_cast<unsigned int>(Keys::K_F24)] 			= win_states[VK_F24] & msb;

		keyStates[static_cast<unsigned int>(Keys::K_LEFTARROW)]		= win_states[VK_LEFT] & msb;
		keyStates[static_cast<unsigned int>(Keys::K_RIGHTARROW)] 	= win_states[VK_RIGHT] & msb;
		keyStates[static_cast<unsigned int>(Keys::K_UPARROW)] 		= win_states[VK_UP] & msb;
		keyStates[static_cast<unsigned int>(Keys::K_DOWNARROW)] 	= win_states[VK_DOWN] & msb;

		keyStates[static_cast<unsigned int>(Keys::K_LSYSTEM)] 		= win_states[VK_LWIN] & msb;
		keyStates[static_cast<unsigned int>(Keys::K_RSYSTEM)] 		= win_states[VK_RWIN] & msb;

		keyStates[static_cast<unsigned int>(Keys::K_SEMICOLON)] 	= win_states[VK_OEM_1] & msb;
		keyStates[static_cast<unsigned int>(Keys::K_FSLASH)] 		= win_states[VK_OEM_2] & msb;
		keyStates[static_cast<unsigned int>(Keys::K_BSLASH)] 		= win_states[VK_OEM_5] & msb;
		keyStates[static_cast<unsigned int>(Keys::K_EQUAL)] 		= win_states[VK_OEM_PLUS] & msb;
		keyStates[static_cast<unsigned int>(Keys::K_DASH)] 			= win_states[VK_OEM_MINUS] & msb;
		keyStates[static_cast<unsigned int>(Keys::K_LBRACKET)] 		= win_states[VK_OEM_4] & msb;
		keyStates[static_cast<unsigned int>(Keys::K_RBRACKET)] 		= win_states[VK_OEM_6] & msb;
		keyStates[static_cast<unsigned int>(Keys::K_COMMA)] 		= win_states[VK_OEM_COMMA] & msb;
		keyStates[static_cast<unsigned int>(Keys::K_PERIOD)] 		= win_states[VK_OEM_PERIOD] & msb;
		keyStates[static_cast<unsigned int>(Keys::K_QUOTE)] 		= win_states[VK_OEM_7] & msb;
		keyStates[static_cast<unsigned int>(Keys::K_TILDE)] 		= win_states[VK_OEM_3] & msb;

		keyStates[static_cast<unsigned int>(Keys::K_A)] 			= win_states['A'] & msb;
		keyStates[static_cast<unsigned int>(Keys::K_B)]				= win_states['B'] & msb;
		keyStates[static_cast<unsigned int>(Keys::K_C)]				= win_states['C'] & msb;
		keyStates[static_cast<unsigned int>(Keys::K_D)]				= win_states['D'] & msb;
		keyStates[static_cast<unsigned int>(Keys::K_E)]				= win_states['E'] & msb;
		keyStates[static_cast<unsigned int>(Keys::K_F)]				= win_states['F'] & msb;
		keyStates[static_cast<unsigned int>(Keys::K_G)]				= win_states['G'] & msb;
		keyStates[static_cast<unsigned int>(Keys::K_H)]				= win_states['H'] & msb;
		keyStates[static_cast<unsigned int>(Keys::K_I)]				= win_states['I'] & msb;
		keyStates[static_cast<unsigned int>(Keys::K_J)]				= win_states['J'] & msb;
		keyStates[static_cast<unsigned int>(Keys::K_K)]				= win_states['K'] & msb;
		keyStates[static_cast<unsigned int>(Keys::K_L)]				= win_states['L'] & msb;
		keyStates[static_cast<unsigned int>(Keys::K_M)]				= win_states['M'] & msb;
		keyStates[static_cast<unsigned int>(Keys::K_N)] 			= win_states['N'] & msb;
		keyStates[static_cast<unsigned int>(Keys::K_O)]				= win_states['O'] & msb;
		keyStates[static_cast<unsigned int>(Keys::K_P)]				= win_states['P'] & msb;
		keyStates[static_cast<unsigned int>(Keys::K_Q)]				= win_states['Q'] & msb;
		keyStates[static_cast<unsigned int>(Keys::K_R)]				= win_states['R'] & msb;
		keyStates[static_cast<unsigned int>(Keys::K_S)]				= win_states['S'] & msb;
		keyStates[static_cast<unsigned int>(Keys::K_T)]				= win_states['T'] & msb;
		keyStates[static_cast<unsigned int>(Keys::K_U)]				= win_states['U'] & msb;
		keyStates[static_cast<unsigned int>(Keys::K_V)]				= win_states['V'] & msb;
		keyStates[static_cast<unsigned int>(Keys::K_W)]				= win_states['W'] & msb;
		keyStates[static_cast<unsigned int>(Keys::K_X)]				= win_states['X'] & msb;
		keyStates[static_cast<unsigned int>(Keys::K_Y)]				= win_states['Y'] & msb;
		keyStates[static_cast<unsigned int>(Keys::K_Z)]				= win_states['Z'] & msb;
		
		keyStates[static_cast<unsigned int>(Keys::K_0)] 			= win_states['0'] & msb;
		keyStates[static_cast<unsigned int>(Keys::K_1)]				= win_states['1'] & msb;
		keyStates[static_cast<unsigned int>(Keys::K_2)]				= win_states['2'] & msb;
		keyStates[static_cast<unsigned int>(Keys::K_3)]				= win_states['3'] & msb;
		keyStates[static_cast<unsigned int>(Keys::K_4)]				= win_states['4'] & msb;
		keyStates[static_cast<unsigned int>(Keys::K_5)]				= win_states['5'] & msb;
		keyStates[static_cast<unsigned int>(Keys::K_6)]				= win_states['6'] & msb;
		keyStates[static_cast<unsigned int>(Keys::K_7)]				= win_states['7'] & msb;
		keyStates[static_cast<unsigned int>(Keys::K_8)]				= win_states['8'] & msb;
		keyStates[static_cast<unsigned int>(Keys::K_9)]				= win_states['9'] & msb;
		
		keyStates[static_cast<unsigned int>(Keys::K_NUMPAD0)] 		= win_states[VK_NUMPAD0] & msb;
		keyStates[static_cast<unsigned int>(Keys::K_NUMPAD1)]		= win_states[VK_NUMPAD1] & msb;
		keyStates[static_cast<unsigned int>(Keys::K_NUMPAD2)]		= win_states[VK_NUMPAD2] & msb;
		keyStates[static_cast<unsigned int>(Keys::K_NUMPAD3)]		= win_states[VK_NUMPAD3] & msb;
		keyStates[static_cast<unsigned int>(Keys::K_NUMPAD4)]		= win_states[VK_NUMPAD4] & msb;
		keyStates[static_cast<unsigned int>(Keys::K_NUMPAD5)]		= win_states[VK_NUMPAD5] & msb;
		keyStates[static_cast<unsigned int>(Keys::K_NUMPAD6)]		= win_states[VK_NUMPAD6] & msb;
		keyStates[static_cast<unsigned int>(Keys::K_NUMPAD7)]		= win_states[VK_NUMPAD7] & msb;
		keyStates[static_cast<unsigned int>(Keys::K_NUMPAD8)]		= win_states[VK_NUMPAD8] & msb;
		keyStates[static_cast<unsigned int>(Keys::K_NUMPAD9)]		= win_states[VK_NUMPAD9] & msb;

	}
	else
		status.setError(AS::ErrorType::GENERAL, "Windows's GetKeyboardState() failed. Could not update key states.");

	delete[] win_states;

	return status;
}