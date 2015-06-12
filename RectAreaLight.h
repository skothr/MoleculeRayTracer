#ifndef RECT_AREA_LIGHT_H
#define RECT_AREA_LIGHT_H

#include "Light.h"
#include "Vector.h"
#include "RectObject.h"

#include <vector>

#define MAX_RECT_AREA_LIGHTS 8

//Defines an area light (shape defined in the same way as a RectObject)
class RectAreaLight : Light
{
protected:
	//The rectangle is defined by 3 points
	Point3f		points[3];
	int			index = -1;

	RectObject	rect;

	static std::vector<RectAreaLight*> lightList;
	static int lightCount;

public:
	RectAreaLight(Point3f p1, Point3f p2, Point3f p3, Color color_);
	virtual ~RectAreaLight();

	void setPoints(Point3f p1, Point3f p2, Point3f p3);
	
	static void updateShaderList(ShaderProgram *pSP);

	virtual void updateShader(ShaderProgram *pSP) override;
};

#endif	//RECT_AREA_LIGHT_H