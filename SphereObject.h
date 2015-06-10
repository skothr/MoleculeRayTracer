#ifndef SPHERE_OBJECT_H
#define SPHERE_OBJECT_H

#include "RayObject.h"
#include "Vector.h"

#include <vector>

#define MAX_SPHERES 90

class ShaderProgram;
class Material;

class SphereObject : public RayObject
{
protected:
	Point3f center;
	float radius;
	bool drawOnly = false;	//If true, this won't cast any shadows.

	int index = -1;

	Material *material = nullptr;

	static std::vector<SphereObject*> sphereList;
	static int sphereCount;

public:
	SphereObject(Point3f center_point, float radius_, Material *mat, bool no_shadows = false);
	virtual ~SphereObject();

	static void updateShaderList(ShaderProgram *pSP);
	static unsigned int numSpheres();
	
	void setCenter(Point3f new_center);
	void moveCenter(Vec3f d_center);
	Point3f getCenter() const;

	void setRadius(float new_radius);
	float getRadius() const;

	virtual void updateShader(ShaderProgram *pSP) override;
};



#endif	//SPHERE_OBJECT_H