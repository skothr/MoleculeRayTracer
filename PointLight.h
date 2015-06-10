#ifndef POINT_LIGHT_H
#define POINT_LIGHT_H

#include "Light.h"
#include "SphereObject.h"
#include <vector>

#define MAX_POINT_LIGHTS 8

class ShaderProgram;

class PointLight : public Light
{
protected:
	Point3f	pos;
	int		index = -1;

	SphereObject sphere;

	static std::vector<PointLight*> lightList;
	static int lightCount;

public:
	PointLight(Point3f pos_, Color color_);
	virtual ~PointLight();

	void setPos(Point3f new_pos);
	void movePos(Vec3f d_pos);

	static void updateShaderList(ShaderProgram *pSP);

	virtual void updateShader(ShaderProgram *pSP) override;
};

#endif	//POINT_LIGHT_H