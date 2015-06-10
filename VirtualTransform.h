#ifndef APOLLO_VIRTUAL_TRANSFORM_H
#define APOLLO_VIRTUAL_TRANSFORM_H

#include "Matrix.h"
#include "Screen.h"
#include "Rect.h"

//Represents a transformation into a container's virtual space
class VirtualTransform
{
private:
	Mat3f	absoluteToVirtual,
			virtualToAbsolute;

	bool	translated = false,	//Whether the current space is translated beyond identity
			scaled = false;		//Whether the current space is scaled beyond identity

public:

	static const Mat3f IDENTITY_MAT;

	VirtualTransform();
	//~VirtualTransform();

	//Functions to transform this virtual space --> transforms VIRTUAL space
	void loadIdentity();
	void translateSpace(Point2f offset);
	void scaleSpace(Vec2f mult);	//TODO: Allow choosing a center point?
	//void rotateSpace(...);

	Mat3f getAbsoluteToVirtualMat() const;
	Mat3f getVirtualToAbsoluteMat() const;

	APoint absoluteToVirtualPoint(APoint p) const;
	AVec absoluteToVirtualVec(AVec v) const;
	Rect absoluteToVirtualRect(Rect r) const;
	
	APoint virtualToAbsolutePoint(APoint p) const;
	AVec virtualToAbsoluteVec(AVec v) const;
	Rect virtualToAbsoluteRect(Rect r) const;
};

#endif	//APOLLO_VIRTUAL_TRANSFORM_H