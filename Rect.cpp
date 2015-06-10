#include "Rect.h"

#define MAX(x0, x1) (x0 >= x1 ? x0 : x1)
#define MIN(x0, x1) (x0 <= x1 ? x0 : x1)

/////RECT/////

Rect::Rect()
{ }
Rect::Rect(APoint pos_, AVec size_)//, TransformSpace t_space)
	: pos(pos_), size(size_)//, space(t_space)
{ }

Rect Rect::findIntersection(const Rect &r1, const Rect &r2)
{
	APoint new_pos = APoint(MAX(r1.x, r2.x), MAX(r1.y, r2.y));
	AVec new_size = AVec(MIN(r1.x + r1.w, r2.x + r2.w), MIN(r1.y + r1.h, r2.y + r2.h)) - new_pos;
	return Rect(new_pos, new_size);//, r1.space);
}

bool Rect::intersects(const Rect &r1, const Rect &r2)
{
	return findIntersection(r1, r2).isValid();
}

bool Rect::contains(const APoint &p) const
{
	return ((p.x >= x && p.x < x + w) &&
			(p.y >= y && p.y < y + h));
}


bool Rect::isValid() const
{
	return w > 0 && h > 0;
}


/////VIEW RECT/////

ViewRect::ViewRect()
{ }
ViewRect::ViewRect(Point2i pos_, Point2i size_)
	: pos(pos_), size(size_)
{ }

ViewRect ViewRect::findIntersection(const ViewRect &r1, const ViewRect &r2)
{
	Point2i new_pos = Point2i(MAX(r1.x, r2.x), MAX(r1.y, r2.y));
	Vec2i new_size = Vec2i(MIN(r1.x + r1.w, r2.x + r2.w), MIN(r1.y + r1.h, r2.y + r2.h)) - new_pos;
	return ViewRect(new_pos, new_size);
}

bool ViewRect::intersects(const ViewRect &r1, const ViewRect &r2)
{
	return findIntersection(r1, r2).isValid();
}

bool ViewRect::contains(const Point2i &p) const
{
	return ((p.x >= x && p.x < x + w) &&
			(p.y >= y && p.y < y + h));
}

bool ViewRect::contains(const APoint &p) const
{
	return ((p.x >= (float)x && p.x < (float)(x + w)) &&
			(p.y >= (float)y && p.y < (float)(y + h)));
}



bool ViewRect::isValid() const
{
	return w > 0 && h > 0;
}



/////EDGES/////

Edges::Edges()
	: l(0), r(0), t(0), b(0)
{ }

Edges::Edges(int left, int right, int top, int bottom)
	: l(left), r(right), t(top), b(bottom)
{ }