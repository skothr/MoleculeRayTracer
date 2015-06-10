#ifndef APOLLO_VIEWPORT_H
#define APOLLO_VIEWPORT_H

#include "Screen.h"

//Represents an openGL viewport
//If isVisible() returns false, all values are invalidated.
struct Viewport
{
	union
	{
		int view[4];

		struct
		{
			int x, y, w, h;
		};
	};

	Viewport();
	Viewport(int x_, int y_, int w_, int h_);

	bool isVisible() const;
	bool setNotVisible();

	//Returns whether the given point is visible in the viewport
	bool contains(const APoint &p);
};


#endif	//APOLLO_VIEWPORT_H