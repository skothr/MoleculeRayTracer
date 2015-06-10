#include "GuiElement.h"

#include "ParentElement.h"
#include "TransformContainer.h"
#include "Window.h"

GuiElement::GuiElement()
	: parent(nullptr), pos(), size(), visible(false),
		leftAttachment(nullptr), rightAttachment(nullptr), topAttachment(nullptr), bottomAttachment(nullptr)
{
	setDefaultBg();
}

GuiElement::GuiElement(ParentElement *parent_, APoint a_pos, AVec a_size, GuiProps flags)
	: parent(parent_), pos(a_pos), size(a_size),
		visible(flags.isVisible()),
		leftAttachment(nullptr), rightAttachment(nullptr), topAttachment(nullptr), bottomAttachment(nullptr)
{
	setDefaultBg();

	Window *this_window = dynamic_cast<Window*>(this);

	//Add this element to parent's children
	if(parent)
	{
		if(this_window)
		{
			parent->getTopWindow()->addChildWindow(this_window);
		}
		else
			parent->addChild(this, flags.isFloating());
	}
}

GuiElement::~GuiElement()
{
	//for(auto a : attachments)
	//	a.element->detach(this);
	//detachAll();

	for(int side = 0; side < Side::COUNT; side++)
		detach((AttachSide)side);
}


void GuiElement::resolveAttachments()
{
	for(auto a : attachments)
	{
		if(a.child)
		{
			//The attached element is a child of this element, so attach it to the inner sides
			switch(a.side)
			{
			case AttachSide::LEFT:
				a.element->setX(a.offset);
				break;
			case AttachSide::RIGHT:
				if(a.element->leftAttachment)
					//Attached on the left too, so stretch it instead of moving it
					a.element->setWidth(size.x - a.element->getPos().x - a.offset);
				else
					//Not attached on the left, so just move it
					a.element->setX(size.x - a.element->getSize().x - a.offset);
				break;
			case AttachSide::TOP:
				a.element->setY(a.offset);
				break;
			case AttachSide::BOTTOM:
				if(a.element->topAttachment)
					//Attached on the top too, so stretch it instead of moving it
					a.element->setHeight(size.y - a.element->getPos().y - a.offset);
				else
					//Not attached on the top, so just move it
					a.element->setY(size.y - a.element->getSize().y - a.offset);
				break;
			}
		}
		else
		{
			//The attached element is not a child of this element, so attach it to the outer sides
			switch(a.side)
			{
			case AttachSide::LEFT:
				a.element->setX(pos.x + size.x + a.offset);
				break;
			case AttachSide::RIGHT:
				if(a.element->leftAttachment)
					//Attached on the left too, so stretch it instead of moving it
					a.element->setWidth(pos.x - a.element->getPos().x - a.offset);
				else
					//Not attached on the left, so just move it
					a.element->setX(pos.x - a.element->getSize().x - a.offset);
				break;
			case AttachSide::TOP:
				a.element->setY(pos.y + size.y + a.offset);
				break;
			case AttachSide::BOTTOM:
				if(a.element->topAttachment)
					//Attached on the top too, so stretch it instead of moving it
					a.element->setHeight(pos.y - a.element->getPos().y - a.offset);
				else
					//Not attached on the top, so just move it
					a.element->setY(pos.y - a.element->getSize().y - a.offset);
				break;
			}
		}

		onResolveAttachment(a.element, a.side);
	}
}

//Attaches the OTHER element to THIS
void GuiElement::attach(GuiAttachment desc)
{
	//Make sure these elements can be attached -- this element must either be its parent or share the same parent
	//	(Otherwise they're not in the same tree level, and can't logically be attached)
	desc.child = (GuiElement*)desc.element->parent == this;

	if(desc.child || desc.element->parent == parent)
	{
		if(desc.element->attachedTo[desc.side] == this)
		{
			//Find attachment, and edit its offset
			for(auto a : attachments)
			{
				if(a.element == desc.element && a.side == desc.side)
				{
					a.offset = desc.offset;
					break;
				}
			}
		}
		else
		{
			desc.element->detach(desc.side);
			desc.element->attachedTo[desc.side] = this;

			attachments.push_back(desc);
			onAttach(attachments[attachments.size() - 1]);
		}
		
		resolveAttachments();
	}
	//else not a valid attachment
}

//Attaches THIS to the MASTER
void GuiElement::attachTo(GuiElement *master, AttachSide side, float offset)
{
	master->attach(GuiAttachment(side, this, offset));
}

bool GuiElement::detach(AttachSide side)
{
	if(isAttached(side))
	{
		getAttached(side)->detach(this);
		return true;
	}
	else
		return false;
}

bool GuiElement::detach(GuiElement *e)
{
	if(e)
	{
		for(unsigned int i = 0; i < attachments.size(); i++)
		{
			GuiElement *other_e = attachments[i].element;
			if(other_e == e)
			{
				other_e->attachedTo[attachments[i].side] = nullptr;
				attachments.erase(attachments.begin() + i);

				return true;
			}
		}
	}

	return false;
}

void GuiElement::detachAll()
{
	//TODO: Need to detach???
	//for(auto a : attachments)
	//	if(a.element)
	//		a.element->attachedTo[a.side] = nullptr;
	attachments.clear();
}

GuiElement* GuiElement::getAttached(AttachSide side)
{
	return attachedTo[side];
}

bool GuiElement::isAttached(AttachSide side)
{
	return (bool)getAttached(side);
}


bool GuiElement::pointInside(APoint p) const
{
	//Default -- calculated just from position/size (clamped to be within parent element)
	Rect r = getRect();//getVisibleRect();//Rect(pos, size);//parent->clampRect(Rect(pos, size));

	return p.x >= r.pos.x && p.x < r.pos.x + r.size.x && p.y >= r.pos.y && p.y < r.pos.y + r.size.y;
}

void GuiElement::updateResources()
{
	onPosChanged(AVec());
	onSizeChanged(AVec());
	resolveAttachments();
}

void GuiElement::drawBackground(GlInterface &gl)
{
	gl.setColor(bgColor);
	gl.drawRect(Rect(APoint(), size));
}

void GuiElement::setDefaultBg()
{
	bgColor = Color(0.15f, 0.15f, 0.15f, 1.0f);
}

Window* GuiElement::getTopWindow()
{
	Window *this_window = dynamic_cast<Window*>(this);
	
	return this_window ? this_window : parent->getTopWindow();
}

void GuiElement::setParent(ParentElement *new_parent, bool was_floating, bool now_floating, bool handle)
{
	if(parent == new_parent)
		return;

	if(handle)
	{
		if(parent)
			parent->removeChild(this, was_floating);

		if(new_parent)
			new_parent->addChild(this, now_floating);	//TODO: Keep track of whether this element is floating!!
	}

	parent = new_parent;

	onParentChanged();
}


void GuiElement::setPos(APoint a_pos)
{
	AVec diff = a_pos - pos;
	pos = a_pos;
	onPosChanged(diff);
	resolveAttachments();
}

void GuiElement::setX(float new_x)
{
	AVec diff(new_x - pos.x, 0.0f);
	pos.x = new_x;
	onPosChanged(diff);
	resolveAttachments();
}

void GuiElement::setY(float new_y)
{
	AVec diff(0.0f, new_y - pos.y);
	pos.y = new_y;
	onPosChanged(diff);
	resolveAttachments();
}

void GuiElement::move(AVec d_pos)
{
	setPos(getPos() + d_pos);
}

APoint GuiElement::getPos() const
{
	return pos;
}

void GuiElement::setSize(AVec a_size)
{
	AVec diff = a_size - size;
	size = a_size;
	onSizeChanged(diff);
	resolveAttachments();
}

void GuiElement::setWidth(float new_w)
{
	AVec diff(new_w - size.x, 0.0f);
	size.x = new_w;
	onSizeChanged(diff);
	resolveAttachments();
}

void GuiElement::setHeight(float new_h)
{
	AVec diff(0.0f, new_h - size.y);
	size.y = new_h;
	onSizeChanged(diff);
	resolveAttachments();
}

AVec GuiElement::getSize() const
{
	return size;
}


void GuiElement::setRect(const Rect &new_rect)
{
	AVec	p_diff = new_rect.pos - pos,
			s_diff = new_rect.size - size;

	pos = new_rect.pos;
	size = new_rect.size;

	onPosChanged(p_diff);
	onSizeChanged(s_diff);
	resolveAttachments();
}

Rect GuiElement::getRect() const
{
	return Rect(pos, size);
}

Rect GuiElement::getVisibleRect() const
{
	return (parent ? parent->clampRect(getRect()) : getRect());
}


bool GuiElement::isVisible() const
{
	return visible;
}

void GuiElement::setVisible(bool is_visible)
{
	if(visible != is_visible)
	{
		visible = is_visible;

		if(visible)
			onShow();
		else
			onHide();
	}
	//Else do nothing -- already correct visibility
}

void GuiElement::show()
{
	setVisible(true);
}

void GuiElement::hide()
{
	setVisible(false);
}

void GuiElement::setBgColor(Color col)
{
	bgColor = col;
}

void GuiElement::setFgColor(Color col)
{
	fgColor = col;
}

const ParentElement* GuiElement::getParent() const
{
	return parent;
}

ParentElement* GuiElement::getParentPtr()
{
	return parent;
}

bool GuiElement::isolateViewport(GlInterface &gl, bool clamp)
{
	TransformContainer *tc = static_cast<TransformContainer*>(parent);

	return (visible && gl.isolateViewport(pos, size, tc ? tc->getViewOffset() : APoint(0.0f, 0.0f), clamp));
}

void GuiElement::restoreViewport(GlInterface &gl)
{
	gl.restoreLast();
}