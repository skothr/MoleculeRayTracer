#include "GuiAttachment.h"

/////GUI ATTACHMENT/////

GuiAttachment::GuiAttachment()
{ }

GuiAttachment::GuiAttachment(AttachSide s, GuiElement *e, float offset_)
	: side(s), element(e), offset(offset_)
{ }