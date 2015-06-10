#include "Window.h"

#include "AUtility.h"
#include "VirtualTransform.h"
//#include "MidiDevice.h"

#include <GL/glew.h>


const GuiPropFlags Window::PROP_FLAGS = PFlags::DRAGGABLE | PFlags::DROPPABLE | PFlags::HARD_BACK | PFlags::ABSORB_SCROLL;

GlInterface Window::gl(ViewRect(APoint(0, 0), AVec(100.0f, 100.0f)));

Window::Window(std::string label_, APoint a_pos, AVec a_size, GuiStateFlags s_flags)
	: GuiElement(nullptr, a_pos, a_size, GuiProps(s_flags, PROP_FLAGS)),
		Container(GuiProps(s_flags, PROP_FLAGS)),
		title(label_)
{
	WindowStyle s;
	s.visible = GuiProps::isVisible(s_flags);

	windowImpl = new WindowWin32(this, nullptr, Vec2i(size), title, s);
	setPos(pos.x, pos.y);
	
	//windowImpl->setResizeFunction(std::bind(&Window::onResize, this, std::placeholders::_1, std::placeholders::_2));

	//Initialize key states
	for(unsigned int i = 0; i < static_cast<unsigned int>(Keys::K_COUNT); i++)
		keyStates[i] = false;

	updateResources();
}

Window::Window(APoint a_pos, AVec a_size, GuiStateFlags s_flags)
	: GuiElement(nullptr, a_pos, a_size, GuiProps(s_flags, PROP_FLAGS)),
		Container(GuiProps(s_flags, PROP_FLAGS)),
		title("")
{
	//setPos(pos.x, pos.y);
	
	//Initialize key states
	for(unsigned int i = 0; i < static_cast<unsigned int>(Keys::K_COUNT); i++)
		keyStates[i] = false;

	//updateResources();
}

Window::~Window()
{
	AU::safeDelete(windowImpl);
}

void Window::loadResources()
{
	//gl = GlInterface();
}


void Window::setPos(int x, int y)
{
	pos.x = x;
	pos.y = y;
	if(windowImpl)
		windowImpl->setPosition(pos);

	updateResources();
}

void Window::setPos(APoint a_pos)
{
	setPos(a_pos.x, a_pos.y);
}

void Window::setSize(int w, int h)
{
	size.x = w;
	size.y = h;
	if(windowImpl)
		windowImpl->setSize(size);

	updateResources();
}

void Window::setSize(AVec a_size)
{
	setSize(a_size.x, a_size.y);
}

APoint Window::getClientPos()
{
	return APoint(windowImpl->getClientPosition());
}

void Window::setFocus(bool in_focus)
{
	windowImpl->setFocus(inFocus);
	ActiveElement::setFocus(in_focus);
}


void Window::onShow()
{
	windowImpl->setVisible(true);
}

void Window::onHide()
{
	windowImpl->setVisible(false);
}

void Window::addChildWindow(Window *child_window)
{
	childWindows.push_back(child_window);
}

void Window::handleWindow()
{
	Mouse::updateStates();
	Keyboard::updateStates();

	for(auto w : childWindows)
		w->windowImpl->handleEvents();

	windowImpl->handleEvents();
}

void Window::handleEvent(const Event &e)
{
	if(windowImpl)
	{
		//Mouse::updateStates();
		//Keyboard::updateStates();

		bool blocked = false;
		int delta;
		int key_index;

		MouseButton buttons;

		AVec d_scroll;
		APoint m_pos;
		AVec d_m_pos;

		static APoint	last_m_pos = Mouse::getPos();

		//Process Event
		switch (e.type)
		{
		case Events::RESIZING:
		case Events::RESIZED:
			setSize(AVec(e.size.w, e.size.h));
			//pos = APoint(e.size.x, e.size.y);
			updateResources();
			break;

		case Events::MAXIMIZED:
			size = AVec(e.size.w, e.size.h);
			//pos = APoint(e.size.w, e.size.y);
			updateResources();
			break;

		case Events::MINIMIZED:
			//TODO: Pause?
			break;

		case Events::CLOSED:			//Window closed
			close();
			break;

		case Events::GAINED_FOCUS:
		case Events::LOST_FOCUS:
			setFocus(e.focus.in_focus);
			break;

		//Mouse stuff
		case EventType::MOUSEBUTTON_DOWN:
			//GuiElement::activeButtons |= e.mouseButton.button;
				
			m_pos = APoint((float)e.mouseButton.x, (float)e.mouseButton.y);
			buttons = e.mouseButton.button;
			
			//std::cout << "MOUSE DOWN -> " << m_pos << " : " << buttons << "\n";

			blocked = false;

			blocked |= floatingChildren.mouseDown(m_pos, buttons, blocked);
			blocked |= bodyChildren.mouseDown(m_pos, buttons, blocked);

			//std::cout << m_pos << ", " << buttons << "\n";

			//if(!blocked)
			//	std::cout << "MOUSE DOWN ON WINDOW BG.\n";
			break;

		case EventType::MOUSEBUTTON_UP:
			//GuiElement::activeButtons &= ~e.mouseButton.button;
				
			m_pos = APoint((float)e.mouseButton.x, (float)e.mouseButton.y);
			buttons = e.mouseButton.button;

			//std::cout << "MOUSE UP -> " << m_pos << " : " << buttons << "\n";

			blocked = false;
			
			blocked |= floatingChildren.mouseUp(m_pos, buttons, blocked);
			blocked |= bodyChildren.mouseUp(m_pos, buttons, blocked);
			
			//if(!blocked)
			//{
			//	if(Mouse::isDragging())
			//	{
			//		std::cout << "DROPPED " << Mouse::getDragElement() << " ONTO WINDOW BG.\n";
			//		Mouse::stopDragging();
			//	}
			//}

			break;
			
		case EventType::MOUSEMOVED:
			m_pos = APoint((float)e.mouseMove.x, (float)e.mouseMove.y);
			d_m_pos = m_pos - last_m_pos;
			last_m_pos = m_pos;

			if(abs(d_m_pos.x) > 0 || abs(d_m_pos.y) > 0)
			{
				//std::cout << m_pos.x << ", " << m_pos.y << "\n";
				blocked = false;

				blocked |= floatingChildren.mouseMove(m_pos, d_m_pos, blocked);
				blocked |= bodyChildren.mouseMove(m_pos, d_m_pos, blocked);
			}

			break;
				
		case Events::MOUSEWHEEL_MOVED:
			//std::cout << e.mouseWheel.delta_y << "\n";
			//std::cout << e.mouseWheel.x << ", " << e.mouseWheel.y << "\n";
				
			m_pos = APoint(e.mouseWheel.x, e.mouseWheel.y);
			d_scroll = AVec(-e.mouseWheel.delta_x, -e.mouseWheel.delta_y);

			blocked = false;

			blocked |= floatingChildren.mouseScroll(m_pos, d_scroll, blocked);
			blocked |= bodyChildren.mouseScroll(m_pos, d_scroll, blocked);

			//if(!blocked)
			//	std::cout << "SCROLLED " << d_scroll.y << " ON WINDOW BG.\n";

			break;
		//Key stuff
		case Events::KEY_DOWN:
			key_index = static_cast<int>(e.key.keyCode);

			if(key_index >= 0 && (keyRepeat || !keyStates[key_index]))
			{
				keyStates[key_index] = true;

				//Send event to MidiDevices for virtual devices
				//MidiDevice::keyEvent(e.key.keyCode, Events::KEY_DOWN);



				switch (e.key.keyCode)
				{
				case Keys::K_S:
					break;
				}

				floatingChildren.keyDown(e.key.keyCode);
				bodyChildren.keyDown(e.key.keyCode);

			}
			break;

		case Events::KEY_UP:
			key_index = static_cast<int>(e.key.keyCode);

			if(key_index >= 0 && (keyRepeat || keyStates[key_index]))
			{
				keyStates[key_index] = false;

				//Send event to MidiDevices for virtual devices
				//MidiDevice::keyEvent(e.key.keyCode, Events::KEY_UP);

				switch(e.key.keyCode)
				{
				case Keys::K_S:
					break;
				}
				
				floatingChildren.keyUp(e.key.keyCode);
				bodyChildren.keyUp(e.key.keyCode);
			}
			break;

		default:
			break;
		}
	}

	/*
	//Handle mouse seperately, to make sure events are global
	//TODO: Look into making windows send events globally
	if(windowImpl)// && isInFocus())
	{
		bool blocked = false;

		//MOUSE MOVED?
		APoint m_pos = APoint(windowImpl->getClientMousePos());
		AVec d_m_pos = AVec(Mouse::getDPos());
		if(d_m_pos.x != 0 || d_m_pos.y != 0)
		{
			std::cout << m_pos.x << ", " << m_pos.y << "\n";
			
			blocked |= floatingChildren.mouseMove(m_pos, d_m_pos, blocked);
			blocked |= bodyChildren.mouseMove(m_pos, d_m_pos, blocked);

			//blocked |= hudChildren.mouseMove(m_pos, d_m_pos, blocked);
			//blocked |= bodyChildren.mouseMove(m_pos, d_m_pos, blocked);
		}

		//MOUSE CLICKED?
		MouseButton down_buttons = Mouse::getButtonsDown(),
					up_buttons = Mouse::getButtonsUp();
		
		//MOUSE BUTTON DOWN
		if(valid(down_buttons))
		{
			blocked = false;
			
			blocked |= floatingChildren.mouseDown(m_pos, down_buttons, blocked);
			blocked |= bodyChildren.mouseDown(m_pos, down_buttons, blocked);

			//blocked |= hudChildren.mouseDown(m_pos, down_buttons, blocked);
			//blocked |= bodyChildren.mouseDown(m_pos, down_buttons, blocked);

			if(!blocked)
				std::cout << "MOUSE DOWN ON WINDOW BG.\n";
		}
		
		//MOUSE BUTTON UP
		if(valid(up_buttons))
		{
			blocked = false;
			
			blocked |= floatingChildren.mouseUp(m_pos, up_buttons, blocked);
			blocked |= bodyChildren.mouseUp(m_pos, up_buttons, blocked);

			//blocked |= hudChildren.mouseUp(m_pos, up_buttons, blocked);
			//blocked |= bodyChildren.mouseUp(m_pos, up_buttons, blocked);
			
			if(!blocked)
			{
				if(Mouse::isDragging())
				{
					std::cout << "DROPPED " << Mouse::getDragElement() << " ONTO WINDOW BG.\n";
					Mouse::stopDragging();
				}
			}
		}
	}
	*/
}

void Window::onResize(Point2i p, Vec2i s)
{
	AVec	p_diff = AVec(p) - pos,
			s_diff = AVec(s) - size;

	pos = APoint((float)p.x, (float)p.y);
	size = AVec((float)s.x, (float)s.y);

	//Resolve attachments
	onPosChanged(p_diff);
	onSizeChanged(s_diff);
	resolveAttachments();

	draw();
}

void Window::update(const Time &dt)
{
	//windowImpl->setActive(windowImpl->getHDC());
	//glClearColor(bgColor.r, bgColor.g, bgColor.b, bgColor.a);
	Container::update(dt);
}

void Window::draw(GlInterface &gl)
{
	//Ignore input (just there for consistency with virtual function)
	draw();
}

void Window::draw()
{
	if(windowImpl && visible)
	{
		windowImpl->setActive();
		GlInterface gl2(ViewRect(APoint(0, 0), size));

		//Draw background
		resetView();
		glClear(GL_COLOR_BUFFER_BIT);

		drawChildren(gl2);
		windowImpl->swapBuffers(windowImpl->getHDC());
	}
}

void Window::resetView()
{
	glClearColor(bgColor.r, bgColor.g, bgColor.b, bgColor.a);
}

bool Window::isOpen()
{
	return open;
}

void Window::close()
{
	open = false;
	
	delete windowImpl;
	windowImpl = nullptr;
}