#ifndef APOLLO_RECT_H
#define APOLLO_RECT_H

//#include "TransformSpace.h"
#include "Screen.h"

struct Rect
{
	union
	{
		struct { APoint pos; };
		struct { float x, y; };
	};
	union
	{
		struct { AVec size; };
		struct { float w, h; };
	};

	//TransformSpace space = TSpace::INVALID;

	Rect();
	Rect(APoint pos_, AVec size_);//, TransformSpace t_space = TSpace::O_ABSOLUTE);

	static Rect findIntersection(const Rect &r1, const Rect &r2);
	static bool intersects(const Rect &r1, const Rect &r2);

	//Returns whether the given point is inside this rect
	bool contains(const APoint &p) const;

	bool isValid() const;
};

//Rect holding the area of a view (using ints)
struct ViewRect
{
	union
	{
		struct { Point2i pos; };
		struct { int x, y; };
	};
	union
	{
		struct { Vec2i size; };
		struct { int w, h; };
	};

	ViewRect();
	ViewRect(Point2i pos_, Vec2i size_);

	static ViewRect findIntersection(const ViewRect &r1, const ViewRect &r2);
	static bool intersects(const ViewRect &r1, const ViewRect &r2);

	//Returns whether the given point is inside this rect
	bool contains(const Point2i &p) const;
	bool contains(const APoint &p) const;

	bool isValid() const;
};


//Holds a value for each edge of a Rect
struct Edges
{
	int l = 0,
		r = 0,
		t = 0,
		b = 0;

	Edges();
	Edges(int left, int right, int top, int bottom);
};




#endif	//APOLLO_RECT_H