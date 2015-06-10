#ifndef MATERIAL_H
#define MATERIAL_H

#include "Color.h"

#include <vector>

class ShaderProgram;

#define MAX_MATERIALS 8

class Material
{
protected:
	Color	ambientColor,
			diffuseColor,
			specularColor,
			emissiveColor;
	float	shininess;

	int		index = -1;

	static std::vector<Material*> materialList;
	static int materialCount;

public:
	Material(Color ambient, Color diffuse, Color specular, Color emissive, float shiny);
	virtual ~Material();

	static Material LIGHT_MATERIAL;

	void setAmbient(Color new_ambient);
	void setDiffuse(Color new_diffuse);
	void setSpecular(Color new_specular);
	void setEmissive(Color new_emissive);
	void setShininess(float new_shininess);

	void setMaterial(Color ambient, Color diffuse, Color specular, Color emissive, float shiny);

	int getIndex() const;

	void updateShader(ShaderProgram *pSP);

	static void updateShaderList(ShaderProgram *pSP);
};

#endif	//MATERIAL_H