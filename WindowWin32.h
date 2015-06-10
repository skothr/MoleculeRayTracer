#ifndef APOLLO_WINDOW_WIN32_H
#define APOLLO_WINDOW_WIN32_H

#include <string>
#include <vector>
#include <Windowsx.h>
#include <functional>

#include "Vector.h"
#include "GlContext.h"
#include "Events.h"
#include "AStatus.h"
#include "Keyboard.h"
#include "Mouse.h"

typedef std::function<void(Point2i pos, Vec2i size)> ResizeCallback;
typedef HWND WindowHandle;

struct WindowStyle;

class Window;

struct WindowClass
{
	WNDCLASSEX	windowClass;
	short fillColor[3];
	unsigned int numWindows = 0;
	std::wstring className;

	bool registered = false;

	WindowClass(WNDCLASSEX &wc = WNDCLASSEX());

	static short defaultFillColor[3];
	static void setDefaultFillColor(float r, float g, float b);

	AStatus setFillColor(float r, float g, float b);
	AStatus registerClass();
	AStatus deregisterClass();
};

class WindowWin32
{
private:
	static GlContext	*context;

	static std::vector<WindowClass*> classes;
	static bool classNameFree(std::wstring class_name);
	static WindowClass* getClass(WindowStyle &style);

	static void updateKeyStates();
	static void updateMouseStates();

	Window				*owner = nullptr;

	WindowClass			*windowClass = nullptr;
	DWORD				windowStyle;
	
	//ResizeCallback		resizeFunc = nullptr;

	WindowWin32			*parentWindow = nullptr;
	std::vector<WindowWin32*> children;

	WindowHandle		m_handle = nullptr;
	HDC					m_hdc = nullptr;
	HCURSOR				m_cursor = nullptr;
	HICON				m_icon = nullptr;
	bool				m_keyRepeatEnabled = true;
	Vec2i				m_lastSize;
	bool				m_resizing = false;
	//UINT16			m_surrogate;
	bool				m_mouseInside = false;

	bool				inFocus = false;

	std::vector<Event>	events;

	static LRESULT CALLBACK gEventCallback(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	//Returns whether the given event should continue to propogate to Windows
	LRESULT processEvent(WindowHandle hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	void pushEvent(Event e);
	Event popEvent();
	
	void handleEvents();

public:
	//TODO: replace visible variable with style visibility
	WindowWin32(Window *owner_, WindowWin32 *parent_window, Vec2i size, std::string title, WindowStyle &style);
	virtual ~WindowWin32();
	
	static WindowClass* createWindowClass(WindowStyle &style, std::string class_name = "");
	static void cleanup();

	Vec2i getSize();
	Point2i getPosition();
	Point2i getClientPosition();

	Point2i getClientMousePos();

	WindowHandle getHandle() const;
	HDC getHDC() const;

	void setSize(Vec2i size);
	void setPosition(Point2i pos);
	void setVisible(bool visible);

	void setActive();

	void setFocus(bool in_focus);
	bool isInFocus();

	GlContext* getContext();

	//void setResizeFunction(ResizeCallback resize_func);
	void swapBuffers(HDC window);

	bool checkEvent(Event &e);

	void close();

	friend class Window;
	friend class ContextWindow;
};


enum class TitleButton : int
{
	NONE		= 0x00,
	MINIMIZE	= 0x01,
	MAXIMIZE	= 0x02,
	CLOSE		= 0x04,
	SYSMENU		= 0x08
};
inline TitleButton operator|(TitleButton b1, TitleButton b2)
{ return static_cast<TitleButton>(static_cast<int>(b1) | static_cast<int>(b2)); }
inline TitleButton operator&(TitleButton b1, TitleButton b2)
{ return static_cast<TitleButton>(static_cast<int>(b1) & static_cast<int>(b2)); }
inline TitleButton& operator|=(TitleButton &b1, TitleButton b2)
{ return b1 = b1 | b2; }
inline TitleButton& operator&=(TitleButton &b1, TitleButton b2)
{ return b1 = b1 & b2; }
inline TitleButton operator~(TitleButton b)
{ return static_cast<TitleButton>(~static_cast<int>(b)); }

enum class WindowBorder
{
	NONE = 0,
	THIN,
	SIZEABLE
};

enum class WindowState
{
	FREE = 0,
	MAXIMIZED,
	MINIMIZED
};


//TODO: Create presets for types of windows
struct WindowStyle
{
	//( SEE: http://msdn.microsoft.com/en-us/library/windows/desktop/ff700543(v=vs.85).aspx )

	//Window Styles
	bool enabled = true;
	bool visible = true;
	bool hasTitleBar = true;
	TitleButton titleButtons = TitleButton::CLOSE | TitleButton::MAXIMIZE | TitleButton::MINIMIZE | TitleButton::SYSMENU;
	WindowBorder border = WindowBorder::SIZEABLE;
	WindowState startingState = WindowState::FREE;

	bool hasTabStop = false;

	bool vScroll = false;
	bool hScroll = false;

	bool popup = false;

	//Extended Window Styles
	bool acceptFileDrop = false;
	bool appWindow = false;
	bool sunkenEdge = false;
	bool composited = false;
	bool contextHelp = false;
	bool controlParent = false;
	bool dialogModalFrame = false;		//Double border?
	bool layered = false;
	bool rightToLeft = false;			//Reading language from right to left (sets a couple properties)
	bool noActivate = false;			//Isn't brought to the foregraound when user clicks it
	bool overlappedWindow = false;		//?
	bool paletteWindow = false;
	bool staticEdge = false;			//For windows that dont accept input?
	bool floatingToolbar = false;

	bool topMost = false;
	bool transparent = false;

	bool raisedEdge = false;

	//Class styles
	bool hRedraw = true;
	bool vRedraw = true;

	WindowStyle()
	{ }

	//TODO: Catch things that dont go together
	bool isValidStyle()
	{
		return true;
	}
	
	//TODO: Combine some into more useful variables and find out what everything does
	DWORD getWindowExStyle()
	{
		DWORD style = 0x00000000L;	//Default (?)

		if(acceptFileDrop)
			style |= WS_EX_ACCEPTFILES;
		if(appWindow)
			style |= WS_EX_APPWINDOW;
		if(sunkenEdge)
			style |= WS_EX_CLIENTEDGE;
		if(composited)
			style |= WS_EX_COMPOSITED;
		if(contextHelp)
			style |= WS_EX_CONTEXTHELP;
		if(controlParent)
			style |= WS_EX_CONTROLPARENT;
		if(dialogModalFrame)
			style |= WS_EX_DLGMODALFRAME;
		if(layered)
			style |= WS_EX_LAYERED;
		if(rightToLeft)
			style |= WS_EX_LAYOUTRTL | WS_EX_LEFTSCROLLBAR | WS_EX_RTLREADING | WS_EX_RIGHT;
		if(noActivate)
			style |= WS_EX_NOACTIVATE;
		if(overlappedWindow)
			style |= WS_EX_OVERLAPPEDWINDOW;
		if(paletteWindow)
			style |= WS_EX_PALETTEWINDOW;
		if(staticEdge)
			style |= WS_EX_STATICEDGE;
		if(floatingToolbar)
			style |= WS_EX_TOOLWINDOW;
		if(topMost)
			style |= WS_EX_TOPMOST;
		if(transparent)
			style |= WS_EX_TRANSPARENT;
		if(raisedEdge)
			style |= WS_EX_WINDOWEDGE;

		return style;
	}

	//TODO: Catch things that dont go together, combine some into more useful variables
	DWORD getWindowStyle()
	{
		DWORD style = 0x00000000L;

		//BORDER
		switch(border)
		{
		case WindowBorder::THIN:
			style |= WS_BORDER;
			break;
		case WindowBorder::SIZEABLE:
			style |= WS_SIZEBOX;
			break;
		default:	//NONE
			break;
		}

		//TITLEBAR
		if(static_cast<int>(titleButtons & TitleButton::CLOSE))
			style |= 0;
		if(static_cast<int>(titleButtons & TitleButton::MAXIMIZE))
			style |= WS_MAXIMIZEBOX;
		if(static_cast<int>(titleButtons & TitleButton::MINIMIZE))
			style |= WS_MINIMIZEBOX;
		if(static_cast<int>(titleButtons & TitleButton::SYSMENU))
			style |= WS_SYSMENU;
		
		if(hasTitleBar)
			style |= WS_CAPTION;
		
		//STARTING STATE
		switch(startingState)
		{
		case WindowState::MAXIMIZED:
			style |= WS_MAXIMIZE;
			break;
		case WindowState::MINIMIZED:
			style |= WS_MINIMIZE;
			break;
		default:	//FREE
			break;
		}

		if(!enabled)
			style |= WS_DISABLED;
		if(visible)
			style |= WS_VISIBLE;

		//SCROLLBARS (???)
		if(hScroll)
			style |= WS_HSCROLL;
		if(vScroll)
			style |= WS_VSCROLL;
		
		if(hScroll || vScroll)
			style |= WS_OVERLAPPEDWINDOW;

		//OTHER
		if(hasTabStop)
			style |= WS_TABSTOP;
		if(popup)
			style |= WS_POPUP;

		return style;
	}

	UINT getClassStyle()
	{
		UINT style = 0U;

		//To make opengl work (?)
		style |= CS_OWNDC;

		if(hRedraw)
			style |= CS_HREDRAW;
		if(vRedraw)
			style |= CS_VREDRAW;

		//TODO: etc

		return style;
	}
};



#endif	//APOLLO_WINDOW_WIN32_H