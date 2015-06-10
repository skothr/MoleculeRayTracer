#ifndef APOLLO_GUI_ATTACHMENT_H
#define APOLLO_GUI_ATTACHMENT_H

class GuiElement;

namespace Side
{
	enum AttachSide
	{
		INVALID = -1,

		LEFT = 0,
		RIGHT,
		TOP,
		BOTTOM,

		COUNT
	};
};
typedef Side::AttachSide AttachSide;

struct GuiAttachment
{
	AttachSide side = Side::INVALID;
	GuiElement *element = nullptr;
	float offset = 0.0f;				//Offset from this element the attached element will be attached to 
	bool child = false;					//Whether the attached element is a direct child of this element
										//	The other option (= false) is they have the same parent.

	GuiAttachment();
	GuiAttachment(AttachSide s, GuiElement *e, float offset_);
};



#endif	//APOLLO_GUI_ATTACHMENT_H