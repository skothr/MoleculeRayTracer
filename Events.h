#ifndef APOLLO_EVENTS_H
#define APOLLO_EVENTS_H

#include <Windows.h>
#include <Windowsx.h>

#include "GlContext.h"
#include "Keyboard.h"
#include "Mouse.h"

enum class EventType
{
	INVALID = -1,

	CLOSED = 0,

	RESIZED,
	RESIZING,
	MAXIMIZED,
	MINIMIZED,


	LOST_FOCUS,
	GAINED_FOCUS,

	TEXT_ENTERED,
	KEY_DOWN,
	KEY_UP,

	MOUSEWHEEL_MOVED,
	MOUSEBUTTON_DOWN,
	MOUSEBUTTON_UP,
	MOUSEMOVED,
	MOUSEENTERED,
	MOUSELEFT,

	COUNT
};
typedef EventType Events;

struct Event
{
	struct SizeEvent
	{
		unsigned int w, h,
					 x, y;
	};
	
	struct KeyEvent
	{
		KeyCode keyCode;
		bool	alt,
				control,
				shift,
				system;
	};

	struct MouseMoveEvent
	{
		int		x,
				y;
	};
	
	struct MouseButtonEvent
	{
		MouseButton button;
		int			x,
					y;
	};
	
	struct MouseWheelEvent
	{
		int			x,
					y;

		float		delta_x,
					delta_y;
	};

	struct FocusEvent
	{
		bool in_focus;
	};
	
	EventType type;

	union
	{
		SizeEvent size;
		KeyEvent key;
		MouseMoveEvent mouseMove;
		MouseButtonEvent mouseButton;
		MouseWheelEvent mouseWheel;
		FocusEvent focus;
	};
};

#endif	//APOLLO_EVENTS_H