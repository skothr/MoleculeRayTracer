#ifndef APOLLO_ACTIVE_ELEMENT_H
#define APOLLO_ACTIVE_ELEMENT_H

#include "GuiElement.h"
#include "Mouse.h"
#include "Keyboard.h"
#include "Screen.h"

//Represents what state an active element is in, relative to events
namespace CS
{
	enum ControlState
	{
		INVALID = 0,

		NONE,				//Default state
		HOVERING,			//Mouse is over the element
		DROP_HOVERING,		//Mouse is dragging something over the element
		CLICKING,			//Mouse down on the element and is still down
		DRAGGING,			//Mouse is dragging the element

		COUNT
	};
};
typedef CS::ControlState ControlState;

class GuiGroup;
class VirtualTransform;
class Container;

//Base class for all Active GUI elements.
//	- Has overrideable functions for handling events
//	- Can be enabled/disabled, and in/out of focus
class ActiveElement : public virtual GuiElement
{
private:
	APoint last_m_pos;

protected:
	ActiveElement(GuiProps flags);
	
	bool			enabled,
					inFocus,

					draggable,
					droppable,
					hardBack,
					absorbScroll,

					uncontained;	//Whether the element's children can be outside bounds

	ControlState	cState = CS::NONE;
	
	Color			bgStateColors[CS::COUNT],
					fgStateColors[CS::COUNT];

	//Denotes whether a click/drag started by clicking down on this element
	//	to determine whether the element should be clocked on mouse up.
	bool			clickSelect = false;

	//Mouse handler functions
	bool mouseMove(APoint m_pos, AVec d_pos, bool blocked);
	bool mouseDown(APoint m_pos, MouseButton b, bool blocked);
	bool mouseUp(APoint m_pos, MouseButton b, bool blocked);
	bool mouseScroll(APoint m_pos, AVec d_scroll, bool blocked);
	
	virtual bool respondToMouse(APoint m_pos)					{ return true; }
	virtual bool respondToClick(APoint m_pos, MouseButton b)	{ return true; }

	//Event action functions (override to define)
	//	- Given mouse pos is relative to this element (element pos is the origin)
	//	- bool direct denotes whether the event occurred directly on the element
	virtual void onMouseMove(APoint m_pos, AVec d_pos, bool direct)			{ }
	virtual void onMouseDown(APoint m_pos, MouseButton b, bool direct)		{ }
	virtual void onMouseUp(APoint m_pos, MouseButton b, bool direct)		{ }
	virtual void onScroll(APoint m_pos, AVec d_scroll, bool direct)			{ }
	virtual void onDrag(APoint m_pos, AVec d_pos, bool direct)				{ }	//Called whenever element is dragged at all

	virtual void onKeyDown(KeyCode key)										{ }
	virtual void onKeyUp(KeyCode key)										{ }

	virtual void onStartDragging(APoint m_pos, AVec d_pos)		{ }	//Only called when starting a drag/drop cycle
	virtual void onDrop(APoint m_pos, ActiveElement *element)	{ }

	virtual void onGainedFocus()								{ }
	virtual void onLostFocus()									{ }

	//Overriding GuiElement functions
	virtual void onPosChanged(AVec d_pos) override;
	virtual void onSizeChanged(AVec d_size) override;

	virtual void drawBackground(GlInterface &gl) override;
	virtual void setDefaultBg() override;

public:
	virtual ~ActiveElement();

	void setFocus(bool focused);
	bool isInFocus() const;

	void setBgStateColor(Color col, ControlState state);
	void setAllBgStateColors(Color col);
	void setFgStateColor(Color col, ControlState state);
	void setAllFgStateColors(Color col);

	virtual void update(const Time &dt) override;

	friend class GuiGroup;
};


#endif	//APOLLO_ACTIVE_ELEMENT_H