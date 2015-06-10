#include "SphereObject.h"
#include "Shaders.h"
#include "Material.h"

#include <string>

/////SPHERE OBJECT/////
std::vector<SphereObject*> SphereObject::sphereList(MAX_SPHERES, nullptr);

int SphereObject::sphereCount = 0;


SphereObject::SphereObject(Point3f center_point, float radius_, Material *mat, bool no_shadows)
	: RayObject(RayObjType::SPHERE),
		center(center_point), radius(radius_), material(mat), drawOnly(no_shadows)
{
	auto iter = std::find(sphereList.begin(), sphereList.end(), nullptr);
	if(iter != sphereList.end())
	{
		index = (iter - sphereList.begin());
		sphereList[index] = this;
		sphereCount++;
	}
	else
		std::cout << "TOO MANY SPHERES! Increase MAX_SPHERES.\n";
}

SphereObject::~SphereObject()
{
	if(index >= 0 && index < MAX_SPHERES)
	{
		sphereList.erase(sphereList.begin() + index);
		sphereList.push_back(nullptr);
		sphereCount--;
	}
}

void SphereObject::updateShaderList(ShaderProgram *pSP)
{
	for(unsigned int i = 0; i < MAX_SPHERES; i++)
	{
		SphereObject *s = sphereList[i];
		if(s)
			s->updateShader(pSP);
		else
			pSP->setUniform1f("sphereList[" + std::to_string(i) + "].radius", 0.0f); 
	}

	pSP->setUniform1i("numSpheres", sphereCount);
}

unsigned int SphereObject::numSpheres()
{
	return sphereCount;
}

void SphereObject::setCenter(Point3f new_center)
{
	center = new_center;
}

void SphereObject::moveCenter(Vec3f d_center)
{
	center += d_center;
}

Point3f SphereObject::getCenter() const
{
	return center;
}

void SphereObject::setRadius(float new_radius)
{
	radius = new_radius;
}

float SphereObject::getRadius() const
{
	return radius;
}

void SphereObject::updateShader(ShaderProgram *pSP)
{
	std::string name = "sphereList[" + std::to_string(index) + "]";

	pSP->setUniform3f(name + ".center", center.x, center.y, center.z);
	pSP->setUniform1f(name + ".radius", radius);
	pSP->setUniform1i(name + ".material", (material ? material->getIndex() : 0));
	pSP->setUniform1i(name + ".drawOnly", (int)drawOnly);
}