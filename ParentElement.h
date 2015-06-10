#ifndef APOLLO_PARENT_ELEMENT_H
#define APOLLO_PARENT_ELEMENT_H

#include "GuiElement.h"
#include "GuiGroup.h"

class Window;

class ParentElement : public virtual GuiElement
{
protected:
	GuiGroup	bodyChildren,		//Body children of this element -- transformed into this element's
									//	virtual space
				floatingChildren;	//Children that float on top of this element -- not transformed,
									//	and infront of the rest of the children
	
	void addChild(GuiElement *child, bool floating);
	bool removeChild(GuiElement *child, bool floating);
	void clearChildren();

	//Called whenever a new element is added -- can override
	virtual void onAddElement()		{ }

	virtual void drawChildren(GlInterface &gl);//, bool transform_body = true);
	
	ParentElement();	//Default constructor for virtual inheritance

public:
	virtual ~ParentElement();

	virtual Rect clampRect(const Rect &r) const;	//Clamps the given VIRTUAL-space rect to the borders of the container

	const GuiGroup* getBodyChildren() const;
	const GuiGroup* getFloatingChildren() const;

	virtual bool isolateViewport(GlInterface &gl, bool clamp = true) override;

	virtual void update(const Time &dt) override;
	virtual void draw(GlInterface &gl) override;

	friend class GuiElement;
};



#endif	//APOLLO_PARENT_ELEMENT_H