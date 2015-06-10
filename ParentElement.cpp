#include "ParentElement.h"

#include "TransformContainer.h"

/////CONTAINER/////

ParentElement::ParentElement()
{ }

ParentElement::~ParentElement()
{ }


void ParentElement::addChild(GuiElement *child, bool floating)
{
	if(floating)
		floatingChildren.addFront(child);
	else
		bodyChildren.addFront(child);

	//child->setParent(this, floating, floating, false);

	onAddElement();
}

bool ParentElement::removeChild(GuiElement *child, bool floating)
{
	return floating ? floatingChildren.remove(child) : bodyChildren.remove(child);
}

void ParentElement::clearChildren()
{
	bodyChildren.clear();
	floatingChildren.clear();
}

Rect ParentElement::clampRect(const Rect &r) const
{
	Rect t_r = r;
	
	if(t_r.x < 0.0f)
	{
		t_r.size.x += t_r.x;
		t_r.x = 0.0f;
	}
	if(t_r.x + t_r.size.x > size.x)
	{
		t_r.size.x = size.x - t_r.pos.x;
	}
	
	if(t_r.y < 0.0f)
	{
		t_r.size.y += t_r.y;
		t_r.y = 0.0f;
	}
	if(t_r.y + t_r.size.y > size.y)
	{
		t_r.size.y = size.y - t_r.pos.y;
	}
	
	return t_r;
}


const GuiGroup* ParentElement::getFloatingChildren() const
{
	return &floatingChildren;
}

const GuiGroup* ParentElement::getBodyChildren() const
{
	return &bodyChildren;
}


bool ParentElement::isolateViewport(GlInterface &gl, bool clamp)
{
	TransformContainer *tc = static_cast<TransformContainer*>(parent);

	return visible && gl.isolateViewport(pos, size, tc ? tc->getViewOffset() : APoint(), clamp);
}

void ParentElement::update(const Time &dt)
{
	floatingChildren.update(dt);
	bodyChildren.update(dt);
}

void ParentElement::drawChildren(GlInterface &gl)//, bool transform_body)
{
	bodyChildren.draw(gl);
	floatingChildren.draw(gl);
}

void ParentElement::draw(GlInterface &gl)
{
	if(isolateViewport(gl, true))
	{
		drawBackground(gl);
		drawChildren(gl);

		restoreViewport(gl);
	}
}