#include "CompoundControl.h"

CompoundControl::CompoundControl(GuiProps flags)
	: Container(flags)
{ }

CompoundControl::~CompoundControl()
{ }

//TODO: Rework the organization of this function
//	OR: Get rid of it? Is it necessary?
void CompoundControl::init(const std::vector<GuiElement*> *body_children, const std::vector<GuiElement*> *floating_children)
{
	if(!initialized)
	{
		if(floating_children)
		{
			floatingChildren.clear();
			floatingChildren.addFront(*floating_children);
		}

		if(body_children)
		{
			bodyChildren.clear();
			bodyChildren.addFront(*body_children);
		}

		initialized = true;
	}
}