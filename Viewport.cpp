#include "Viewport.h"

Viewport::Viewport()
	: x(0), y(0), w(0), h(0)
{ }

Viewport::Viewport(int x_, int y_, int w_, int h_)
	: x(x_), y(y_), w(w_), h(h_)
{ }

bool Viewport::isVisible() const
{
	return (w > 0 && h > 0);
}

bool Viewport::setNotVisible()
{
	w = 0;
	h = 0;

	//TODO: return value?
	return true;
}

bool Viewport::contains(const APoint & p)
{
	return (p.x >= (float)x && p.x < (float)(x + w)) &&
			(p.y >= (float)y && p.y < (float)(y + h));
}