#include "SimpleContainers.h"

/////AREA/////
const GuiPropFlags Area::PROP_FLAGS = PFlags::NONE;

Area::Area(ParentElement *parent_, APoint a_pos, AVec a_size, GuiStateFlags s_flags)
	: GuiElement(parent_, a_pos, a_size, GuiProps(s_flags, PROP_FLAGS)),
		Container(GuiProps(s_flags, PROP_FLAGS))
{ }

Area::~Area()
{ }


void Area::update(const Time &dt)
{
	Container::update(dt);
}

void Area::draw(GlInterface &gl)
{
	if(isolateViewport(gl))
	{
		drawBackground(gl);
		drawChildren(gl);

		restoreViewport(gl);
	}
}