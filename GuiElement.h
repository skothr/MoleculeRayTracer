#ifndef APOLLO_GUI_ELEMENT_H
#define APOLLO_GUI_ELEMENT_H

#include "Timing.h"
#include "Screen.h"
#include "Color.h"
#include "GuiProperties.h"
#include "GlInterface.h"
#include "GuiAttachment.h"


class ParentElement;
class Window;

//////////
//TODO: Allow any GuiElement to have child display elements...??
//////////

//Base class for all elements of the GUI.
//	- Has a position, a size, and a visibility
//	- Must override draw function to draw element as desired.
class GuiElement
{
protected:
	GuiElement();
	GuiElement(ParentElement *parent_, APoint a_pos, AVec a_size, GuiProps flags);

	ParentElement	*parent = nullptr;

	APoint pos;
	AVec size;

	bool	visible;		//Whether events pass through this element to elements further back
	
	Color	bgColor,
			fgColor;

	//Each attachment represents another element being "pulled" by any changes in this element
	std::vector<GuiAttachment> attachments;

	union
	{
		GuiElement* attachedTo[AttachSide::COUNT];
		struct
		{
			GuiElement	*leftAttachment,
						*rightAttachment,
						*topAttachment,
						*bottomAttachment;
		};
	};
	
	void resolveAttachments();

	//Override to determine behavior when changing visibility
	//	- Called when VISIBILITY changes -- not necessarily (or only) on show()/hide() function calls
	virtual void onShow()												{ }
	virtual void onHide()												{ }
	
	virtual void onPosChanged(AVec d_pos)								{ }
	virtual void onSizeChanged(AVec d_size)								{ }
	virtual void onParentChanged()										{ }

	virtual void onResolveAttachment(GuiElement *other, AttachSide s)	{ }
	virtual void onAttach(GuiAttachment &attachment)					{ }	//To make any adjustments to the attachment

	virtual bool pointInside(APoint p) const;	//Whether the given point is overlapping this element (can override)

	virtual void drawBackground(GlInterface &gl);

	virtual void setDefaultBg();

public:
	virtual ~GuiElement();
	
	void attach(GuiAttachment desc);
	void attachTo(GuiElement *master, AttachSide side, float offset);
	bool detach(GuiElement *e);
	bool detach(AttachSide side);
	GuiElement* getAttached(AttachSide side);
	bool isAttached(AttachSide side);

	void detachAll();
	
	//handle --> whether the parent knows about the child
	//TODO: Should be protected?
	void setParent(ParentElement *new_parent, bool was_floating, bool now_floating, bool handle = true);
	
	Window* getTopWindow();

	void setPos(APoint a_pos);
	void setX(float new_x);
	void setY(float new_y);
	void move(AVec d_pos);
	APoint getPos() const;

	void setSize(AVec a_size);
	void setWidth(float new_w);
	void setHeight(float new_h);
	AVec getSize() const;

	void setRect(const Rect &new_rect);
	Rect getRect() const;
	Rect getVisibleRect() const;	//Returns the part of this element's rect that can be seen within its parent

	//Updates position and size
	void updateResources();

	bool isVisible() const;
	void setVisible(bool is_visible);
	void show();
	void hide();

	void setBgColor(Color col);
	void setFgColor(Color col);

	const ParentElement* getParent() const;
	ParentElement* getParentPtr();

	virtual bool isolateViewport(GlInterface &gl, bool clamp = true);
	void restoreViewport(GlInterface &gl);

	virtual void update(const Time &dt)	{ }
	virtual void draw(GlInterface &gl) = 0;
};

#endif	//APOLLO_GUI_ELEMENT_H