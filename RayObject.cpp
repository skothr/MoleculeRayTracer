#include "RayObject.h"
#include "Shaders.h"
/////RAY OBJECT/////

RayObject::RayObject(RayObjectType obj_type)
	: type(obj_type)
{

}

RayObject::~RayObject()
{

}


RayObjectType RayObject::getType() const
{
	return type;
}