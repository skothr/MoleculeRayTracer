#include "VirtualTransform.h"

static const float ident_data[9] = {1, 0, 0, 0, 1, 0, 0, 0, 1};

/////VIRTUAL TRANSFORM/////
Mat3f const VirtualTransform::IDENTITY_MAT((float*)ident_data, true);

VirtualTransform::VirtualTransform()
{
	loadIdentity();
}

//Functions to transform this virtual space --> transforms VIRTUAL space
void VirtualTransform::loadIdentity()
{
	absoluteToVirtual.loadIdentity();
	virtualToAbsolute.loadIdentity();
	translated = false;
	scaled = false;
}

void VirtualTransform::translateSpace(Point2f offset)
{
	static Mat3f t;

	t.loadTranslate(offset.x, offset.y);
	//absoluteToVirtual.translate(offset.x, offset.y);
	absoluteToVirtual = absoluteToVirtual * t;
	
	t.loadTranslate(-offset.x, -offset.y);
	//virtualToAbsolute.translate(-offset.x, -offset.y);
	virtualToAbsolute = t * virtualToAbsolute;

	translated = true;
}

void VirtualTransform::scaleSpace(Vec2f mult)
{
	static Mat3f s;

	s.loadScale(1.0f/mult.x, 1.0f/mult.y);
	//absoluteToVirtual.scale(1.0f/mult.x, 1.0f/mult.y);
	absoluteToVirtual = absoluteToVirtual * s;

	s.loadScale(mult.x, mult.y);
	//virtualToAbsolute.scale(mult.x, mult.y);
	virtualToAbsolute = s * virtualToAbsolute;

	scaled = true;
}


Mat3f VirtualTransform::getAbsoluteToVirtualMat() const
{
	return absoluteToVirtual;
}

Mat3f VirtualTransform::getVirtualToAbsoluteMat() const
{
	return virtualToAbsolute;
}


APoint VirtualTransform::absoluteToVirtualPoint(APoint p) const
{
	Vec3f t_p = absoluteToVirtual*Vec3f(p.x, p.y, 1.0f);
	return APoint(t_p.x, t_p.y);
}

AVec VirtualTransform::absoluteToVirtualVec(AVec v) const
{
	Vec3f t_v = absoluteToVirtual*Vec3f(v.x, v.y, 0.0f);
	return APoint(t_v.x, t_v.y);
}

Rect VirtualTransform::absoluteToVirtualRect(Rect r) const
{
	return Rect(absoluteToVirtualPoint(r.pos), absoluteToVirtualVec(r.size));
}

	
APoint VirtualTransform::virtualToAbsolutePoint(APoint p) const
{
	Vec3f t_p = virtualToAbsolute*Vec3f(p.x, p.y, 1.0f);
	return APoint(t_p.x, t_p.y);
}

AVec VirtualTransform::virtualToAbsoluteVec(AVec v) const
{
	Vec3f t_v = virtualToAbsolute*Vec3f(v.x, v.y, 0.0f);
	return APoint(t_v.x, t_v.y);
}

Rect VirtualTransform::virtualToAbsoluteRect(Rect r) const
{
	return Rect(virtualToAbsolutePoint(r.pos), virtualToAbsoluteVec(r.size));
}
