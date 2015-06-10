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

class Light
{
protected:
	LightType	type;
	Color		color;

public:
	Light(LightType Type, Color color_);
	virtual ~Light();

	void setColor(Color new_color);

	virtual void updateShader(ShaderProgram *pSP) = 0;

	LightType getType() const;
};

#endif	//LIGHT_H