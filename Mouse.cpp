#include "Mouse.h"

#include "ActiveElement.h"

/////MOUSE/////
MouseButton			Mouse::buttonStates			= MB::NONE,
					Mouse::lastButtonStates		= MB::NONE,
					Mouse::changedButtonStates	= MB::NONE;

Point2i				Mouse::pos					= Point2i(0, 0),
					Mouse::lastPos				= Point2i(0, 0);

ActiveElement		*Mouse::dragElement			= nullptr;

AStatus Mouse::updateStates()
{
	AStatus status;

	////Buttons////

	//Reset states
	lastButtonStates = buttonStates;
	buttonStates = MB::NONE;

	//If most-significant bit is set, button is down.
	static const SHORT msb = 0x8000;

	//Query states via windows
	if(GetKeyState(VK_LBUTTON) & msb)
		buttonStates |= MB::LEFT;
	if(GetKeyState(VK_RBUTTON) & msb)
		buttonStates |= MB::RIGHT;
	if(GetKeyState(VK_MBUTTON) & msb)
		buttonStates |= MB::MIDDLE;
	if(GetKeyState(VK_XBUTTON1) & msb)
		buttonStates |= MB::X1;
	if(GetKeyState(VK_XBUTTON2) & msb)
		buttonStates |= MB::X2;

	changedButtonStates = buttonStates ^ lastButtonStates;

	////Pos////
	
	POINT p;
	if(GetCursorPos(&p))
	{
		lastPos = pos;
		pos = Point2i(p.x, p.y);
	}
	else
		status.setError(AS::ErrorType::GENERAL, "Failed to get mouse pos!\n");

	return status;
}

bool Mouse::buttonDown(MouseButton b)
{
	return (b != MB::INVALID && static_cast<bool>(b & buttonStates));
}

MouseButton Mouse::getButtonsDown()
{
	return buttonStates & changedButtonStates;
}

MouseButton Mouse::getButtonsUp()
{
	return ~buttonStates & changedButtonStates;
}

MouseButton Mouse::getButtonStates()
{
	return buttonStates;
}

Point2i Mouse::getPos()
{
	return pos;
}

Vec2i Mouse::getDPos()
{
	return pos - lastPos;
}

bool Mouse::isDragging()
{
	return static_cast<bool>(dragElement);
}

void Mouse::startDragging(ActiveElement *element)
{
	dragElement = element;
}

void Mouse::stopDragging()
{
	dragElement = nullptr;
}

ActiveElement* Mouse::getDragElement()
{
	return dragElement;
}