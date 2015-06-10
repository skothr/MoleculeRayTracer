#include "Light.h"


/////LIGHT/////

Light::Light(LightType light_type, Color color_)
	: type(light_type), color(color_)
{ }

Light::~Light()
{ }

void Light::setColor(Color new_color)
{
	color = new_color;
}

LightType Light::getType() const
{
	return type;
}