#include "Material.h"

#include "Shaders.h"

/////MATERIAL/////
std::vector<Material*>	Material::materialList(MAX_MATERIALS, nullptr);
int						Material::materialCount;
Material				Material::LIGHT_MATERIAL(Color(0.5f, 0.5f, 0.5f), Color(0.0f, 0.0f, 0.0f), Color(0.0f, 0.0f, 0.0f), Color(1.0f, 1.0f, 1.0f), 1.0f);

Material::Material(Color ambient, Color diffuse, Color specular, Color emissive, float shiny)
	: ambientColor(ambient), diffuseColor(diffuse), specularColor(specular), emissiveColor(emissive), shininess(shiny)
{
	auto iter = std::find(materialList.begin(), materialList.end(), nullptr);
	if(iter != materialList.end())
	{
		index = (iter - materialList.begin());
		materialList[index] = this;
		materialCount++;
	}
	else
		std::cout << "TOO MANY MATERIALS! Increase MAX_MATERIALS.\n";
}

Material::~Material()
{
	if(index >= 0 && index < MAX_MATERIALS)
	{
		materialList.erase(materialList.begin() + index);
		materialList.push_back(nullptr);
		materialCount--;
	}
}

void Material::setAmbient(Color new_ambient)
{
	ambientColor = new_ambient;
}

void Material::setDiffuse(Color new_diffuse)
{
	diffuseColor = new_diffuse;
}

void Material::setSpecular(Color new_specular)
{
	specularColor = new_specular;
}

void Material::setEmissive(Color new_emissive)
{
	emissiveColor = new_emissive;
}

void Material::setShininess(float new_shininess)
{
	shininess = new_shininess;
}

void Material::setMaterial(Color ambient, Color diffuse, Color specular, Color emissive, float shiny)
{
	ambientColor = ambient;
	diffuseColor = diffuse;
	specularColor = specular;
	emissiveColor = emissive;
	shininess = shiny;
}

int Material::getIndex() const
{
	return index;
}

void Material::updateShader(ShaderProgram *pSP)
{
	std::string name = "materials[" + std::to_string(index) + "]";

	pSP->setUniform3f(name + ".ambient", ambientColor.r, ambientColor.g, ambientColor.b);
	pSP->setUniform3f(name + ".diffuse", diffuseColor.r, diffuseColor.g, diffuseColor.b);
	pSP->setUniform3f(name + ".specular", specularColor.r, specularColor.g, specularColor.b);
	pSP->setUniform3f(name + ".emissive", emissiveColor.r, emissiveColor.g, emissiveColor.b);
	pSP->setUniform1f(name + ".shininess", shininess);
}

void Material::updateShaderList(ShaderProgram *pSP)
{
	for(unsigned int i = 0; i < MAX_MATERIALS; i++)
	{
		Material *m = materialList[i];
		if(m)
			m->updateShader(pSP);
		else
		{
			pSP->setUniform3f("materials[" + std::to_string(i) + "].ambient", 0.0f, 0.0f, 0.0f); 
			pSP->setUniform3f("materials[" + std::to_string(i) + "].diffuse", 0.0f, 0.0f, 0.0f); 
			pSP->setUniform3f("materials[" + std::to_string(i) + "].specular", 0.0f, 0.0f, 0.0f); 
			pSP->setUniform3f("materials[" + std::to_string(i) + "].emissive", 0.0f, 0.0f, 0.0f); 
		}
	}

	pSP->setUniform1i("numMaterials", materialCount);
}