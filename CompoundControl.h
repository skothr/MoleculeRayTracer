#ifndef APOLLO_COMPOUND_CONTROL_H
#define APOLLO_COMPOUND_CONTROL_H

#include "GuiElement.h"
#include "ParentElement.h"
#include "Container.h"

//Like a Container, except its elements are fixed. (TODO: Check that they are)
//	- This allows "Controls" that are made up of different parts.
class CompoundControl : public Container
{
private:
	bool initialized = false;

protected:
	CompoundControl(GuiProps flags);

	void init(const std::vector<GuiElement*> *body_children, const std::vector<GuiElement*> *hud_children);

public:
	virtual ~CompoundControl();
};


#endif	//APOLLO_COMPOUND_CONTROL_H