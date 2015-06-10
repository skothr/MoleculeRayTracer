#include "PointLight.h"

#include "Shaders.h"
#include "Material.h"

/////POINT LIGHT/////

std::vector<PointLight*> PointLight::lightList(MAX_POINT_LIGHTS, nullptr);
int PointLight::lightCount = 0;

PointLight::PointLight(Point3f pos_, Color color_)
	: Light(LightType::POINT, color_),
		pos(pos_), sphere(pos_, 0.2f, &Material::LIGHT_MATERIAL, true)
{
	auto iter = std::find(lightList.begin(), lightList.end(), nullptr);
	if(iter != lightList.end())
	{
		index = (iter - lightList.begin());
		lightList[index] = this;
		lightCount++;
	}
	else
		std::cout << "TOO MANY POINT LIGHTS! Increase MAX_POINT_LIGHTS.\n";
}

PointLight::~PointLight()
{
	if(index >= 0 && index < MAX_POINT_LIGHTS)
	{
		lightList.erase(lightList.begin() + index);
		lightList.push_back(nullptr);
		lightCount--;
	}
}

void PointLight::setPos(Point3f new_pos)
{
	pos = new_pos;
}

void PointLight::movePos(Vec3f d_pos)
{
	pos += d_pos;
}

void PointLight::updateShaderList(ShaderProgram *pSP)
{
	for(unsigned int i = 0; i < MAX_POINT_LIGHTS; i++)
	{
		Light *l = lightList[i];
		if(l)
			l->updateShader(pSP);
		else
			pSP->setUniform3f("pointLights[" + std::to_string(i) + "].color", 0.0f, 0.0f, 0.0f); 
	}

	pSP->setUniform1i("numPointLights", lightCount);
}

void PointLight::updateShader(ShaderProgram *pSP)
{
	std::string name = "pointLights[" + std::to_string(index) + "]";
	
	pSP->setUniform3f(name + ".pos", pos.x, pos.y, pos.z);
	pSP->setUniform3f(name + ".color", color.r, color.g, color.b);
}