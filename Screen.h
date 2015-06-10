#ifndef APOLLO_SCREEN_H
#define APOLLO_SCREEN_H

#include "Vector.h"

//Absolute coordinates
typedef Point2f APoint;
typedef Vec2f AVec;

//Relative coordinates
typedef Point2f RPoint;
typedef Vec2f RVec;

//Gl coordinates
typedef Point2f GlPoint;
typedef Vec2f GlVec;

////////--------------------------------------
//	Relative:
//		float from (0.0, 0.0) to (1.0, 1.0)
////////
//	Absolute:
//		int from (0, 0) to (WIDTH, HEIGHT)
////////--------------------------------------

////POINTS////

inline float toAbsoluteValue(float rel, float container_size)
{ return rel*container_size; }

inline float toRelativeValue(float ab, float container_size)
{ return ab/container_size; }

//Converts relative coordinates to absolute coordinates.
inline APoint toAbsolutePoint(RPoint relative, AVec screen_size)
{
	return APoint(toAbsoluteValue(relative.x, screen_size.x), toAbsoluteValue(relative.y, screen_size.y));
}

//Converts absolute coordinates to relative coordinates.
inline RPoint toRelativePoint(APoint absolute, AVec screen_size)
{
	return RPoint(toRelativeValue(absolute.x, screen_size.x), toRelativeValue(absolute.y, screen_size.y));
}

//Converts relative coordinates to gl coordinates.
//	screen_size not needed, but added for consistency.
inline GlPoint toGlPoint(RPoint relative)
{
	return GlPoint((relative.x - 0.5f)*2.0f, (relative.y - 0.5f)*-2.0f);
}

//Converts absolute coordinates to gl coordinates.
inline GlPoint toGlPoint(APoint absolute, AVec screen_size)
{
	return toGlPoint(toRelativePoint(absolute, screen_size));
}

////VECTORS////

//Converts relative coordinates to absolute coordinates.
inline AVec toAbsoluteVec(RVec relative, AVec screen_size)
{
	return AVec(toAbsoluteValue(relative.x, screen_size.x), toAbsoluteValue(relative.y, screen_size.y));
}

//Converts absolute coordinates to relative coordinates.
inline RVec toRelativeVec(AVec absolute, AVec screen_size)
{
	return RVec(toRelativeValue(absolute.x, screen_size.x), toRelativeValue(absolute.y, screen_size.y));
}

//Converts relative coordinates to gl coordinates.
//	screen_size not needed, but added for consistency.
/*
inline GlVec toGlVec(RVec relative, AVec screen_size = AVec())
{
	return GlVec(relative.x*2.0f, relative.y*-2.0f);
}
*/

//Converts absolute coordinates to gl coordinates.
inline GlVec toGlVec(AVec absolute, AVec screen_size)
{
	RVec r = toRelativeVec(absolute, screen_size);
	return GlVec(r.x*2.0f, r.y*-2.0f);
}


enum CoordinateMode
{
	ABSOLUTE_COORDS = 0,
	RELATIVE_COORDS,
	INVALID_COORDS
};


#endif	//APOLLO_SCREEN_H