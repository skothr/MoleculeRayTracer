#ifndef APOLLO_MOUSE_H
#define APOLLO_MOUSE_H

#include <Windows.h>

#include "ExpandEnum.h"
#include "AStatus.h"

#include "Vector.h"

enum class MouseButton
{
	INVALID = -1,
	NONE = 0x00,
	LEFT = 0x01,
	RIGHT = 0x02,
	MIDDLE = 0x04,
	X1 = 0x08,
	X2 = 0x10
};
typedef MouseButton MB;

EXPAND_ENUM_CLASS_OPERATORS(MouseButton)
EXPAND_ENUM_CLASS_VALID(MouseButton, MB::NONE)
EXPAND_ENUM_CLASS_PRINT( MouseButton,
						 ({MB::INVALID, MB::NONE, MB::LEFT, MB::RIGHT, MB::MIDDLE, MB::X1, MB::X2}),
						 ({"INVALID", "NONE", "LEFT", "RIGHT", "MIDDLE", "X1", "X2"}) );

class ActiveElement;

class Mouse
{
private:
	static MouseButton			buttonStates,
								lastButtonStates,
								changedButtonStates;

	static Point2i				pos,
								lastPos;

	static ActiveElement		*dragElement;	//Points to the element that is currently being dragged (or nullptr)

	Mouse() { }

public:
	//Updates buttonStates, lastButtonStates, changedButtonStates, pos, and lastPos
	static AStatus updateStates();

	//Returns whether the given button(s) is/are down.
	static bool buttonDown(MouseButton b);
	
	//Returns the buttons that went down within the last update
	static MouseButton getButtonsDown();
	//Returns the buttons that went up within the last update
	static MouseButton getButtonsUp();

	static MouseButton getButtonStates();


	//Returns the position of the mouse on the screen
	static Point2i getPos();
	//Returns the amount the mouse moved in the past update
	static Vec2i getDPos();

	static bool isDragging();
	static void startDragging(ActiveElement *element);
	static void stopDragging();

	static ActiveElement* getDragElement();
};


inline MouseButton convertWindowsMouseButton(UINT winMB)
{
	MouseButton b = MB::NONE;

	if(winMB & MK_LBUTTON)
		b |= MB::LEFT;
	if(winMB & MK_RBUTTON)
		b |= MB::RIGHT;
	if(winMB & MK_MBUTTON)
		b |= MB::MIDDLE;
	if(winMB & MK_XBUTTON1)
		b |= MB::X1;
	if(winMB & MK_XBUTTON2)
		b |= MB::X2;

	//std::cout << std::hex << winMB << "\n";

	return b;
}


#endif	//APOLLO_MOUSE_H