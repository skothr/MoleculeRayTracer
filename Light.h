#ifndef LIGHT_H
#define LIGHT_H

#include "Color.h"

enum class LightType
{
	INVALID = -1,

	POINT = 0,
	AREA_RECT = 1,
	AREA_SPHERE = 2,
};

class ShaderProgram;


//Base class for lights.
//	- All lights have a Color, and a constant to tell what kind of light it is.

class Light
{
protected:
	const LightType	type;
	Color		color;

public:
	Light(LightType Type, Color color_);
	virtual ~Light();

	void setColor(Color new_color);

	virtual void updateShader(ShaderProgram *pSP) = 0;

	LightType getType() const;
};

#endif	//LIGHT_H