#ifndef APOLLO_WINDOW_H
#define APOLLO_WINDOW_H

#include <vector>
#include <string>

#include "Vector.h"
#include "Container.h"

#include "WindowWin32.h"
#include "GlInterface.h"


class Window : public Container
{
protected:
	std::vector<Window*> childWindows;

	std::string title;
	WindowWin32 *windowImpl = nullptr;

	//Used for all windows
	static GlInterface gl;
	
	bool		keyStates[static_cast<unsigned int>(Keys::K_COUNT)];

	bool		open = true;
	bool		keyRepeat = false;

	//virtual void onPosChanged(AVec d_pos) override;
	//virtual void onSizeChanged(AVec d_size) override;

	virtual void onShow() override;
	virtual void onHide() override;



public:
	Window(std::string label, APoint a_pos, AVec a_size, GuiStateFlags s_flags);
	Window(APoint a_pos, AVec a_size, GuiStateFlags s_flags);
	virtual ~Window();

	static const GuiPropFlags PROP_FLAGS;

	static void loadResources();

	void setPos(int x, int y);
	void setPos(APoint a_pos);
	void setSize(int w, int h);
	void setSize(AVec a_size);

	APoint getClientPos();

	virtual void setFocus(bool in_focus);

	bool isOpen();
	void close();

	void addChildWindow(Window *child_window);

	void handleEvent(const Event &e);
	void handleWindow();
	virtual void onResize(Point2i pos, Vec2i size);

	WindowWin32* getWindowImpl() { return windowImpl; }

	virtual void update(const Time &dt) override;

	virtual void draw();
	virtual void draw(GlInterface &gl) override;

	//Resets gl and clearcolor for after another window has been set active
	void resetView();

	friend class ContextWindow;
};


#endif	//APOLLO_WINDOW_H