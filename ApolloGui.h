#ifndef APOLLO_GUI_H
#define APOLLO_GUI_H

#include "GuiElement.h"
#include "ActiveElement.h"
#include "StaticElement.h"

#include "Container.h"
#include "Control.h"
#include "Display.h"
#include "TransformContainer.h"
//#include "GraphDisplay.h"

#include "SimpleContainers.h"
#include "ScrollArea.h"
//#include "MenuBar.h"

#include "Label.h"
#include "Buttons.h"
#include "Checkbox.h"
#include "RadioButtonGroup.h"
#include "CollapseList.h"

#include "Window.h"
#include "Text.h"
#include "Texture.h"

#include "Mouse.h"
#include "Keyboard.h"

#include <GL/glew.h>

inline void loadGuiResources()
{
	Mouse::updateStates();
	Keyboard::updateStates();

	Window::loadResources();

	Font::loadResources();
	Text::loadResources();
	Texture::loadResources();

	Label::initResources();

	ImageButton::loadResources();
}

inline void cleanupGui()
{
	Texture::cleanup();
	WindowWin32::cleanup();
}

inline void setVSync(bool vsync_on)
{
	typedef bool (APIENTRY *PFNWGLSWAPINTERVALFARPROC)(int);
	PFNWGLSWAPINTERVALFARPROC wglSwapIntervalEXT = 0;
	wglSwapIntervalEXT = 
		(PFNWGLSWAPINTERVALFARPROC)wglGetProcAddress("wglSwapIntervalEXT");
	if (wglSwapIntervalEXT)
	wglSwapIntervalEXT(vsync_on ? 1 : 0);
}

#endif	//APOLLO_GUI_H