#include "RectAreaLight.h"

#include "Shaders.h"
#include "Material.h"

/////RECT AREA LIGHT/////
std::vector<RectAreaLight*> RectAreaLight::lightList(MAX_RECT_AREA_LIGHTS, nullptr);
int							RectAreaLight::lightCount = 0;

RectAreaLight::RectAreaLight(Point3f p1, Point3f p2, Point3f p3, Color color_)
	: Light(LightType::AREA_RECT, color_),
		rect(p1, p2, p3, &Material::LIGHT_MATERIAL, true)
{
	points[0] = p1;
	points[1] = p2;
	points[2] = p3;

	auto iter = std::find(lightList.begin(), lightList.end(), nullptr);
	if(iter != lightList.end())
	{
		index = (iter - lightList.begin());
		lightList[index] = this;
		lightCount++;
	}
	else
		std::cout << "TOO MANY RECT AREA LIGHTS! Increase MAX_RECT_AREA_LIGHTS.\n";
}

RectAreaLight::~RectAreaLight()
{
	if(index >= 0 && index < MAX_RECT_AREA_LIGHTS)
	{
		lightList.erase(lightList.begin() + index);
		lightList.push_back(nullptr);
		lightCount--;
	}
}

void RectAreaLight::setPoints(Point3f p1, Point3f p2, Point3f p3)
{
	points[0] = p1;
	points[1] = p2;
	points[2] = p3;
}
	
void RectAreaLight::updateShaderList(ShaderProgram *pSP)
{
	for(unsigned int i = 0; i < MAX_RECT_AREA_LIGHTS; i++)
	{
		Light *l = lightList[i];
		if(l)
			l->updateShader(pSP);
		else
			pSP->setUniform3f("rectAreaLights[" + std::to_string(i) + "].color", 0.0f, 0.0f, 0.0f); 
	}

	pSP->setUniform1i("numRectAreaLights", lightCount);
}

void RectAreaLight::updateShader(ShaderProgram *pSP)
{
	std::string name = "rectAreaLights[" + std::to_string(index) + "]";

	float point_values[] = {points[0].x, points[0].y, points[0].z,
							points[1].x, points[1].y, points[1].z,
							points[2].x, points[2].y, points[2].z};
	
	pSP->setUniform3fv(name + ".points[0]", 3, point_values);
	//pSP->setUniform3f(name + ".points[1]", points[1].x, points[1].y, points[1].z);
	//pSP->setUniform3f(name + ".points[2]", points[2].x, points[2].y, points[2].z);
	pSP->setUniform3f(name + ".color", color.r, color.g, color.b);
}