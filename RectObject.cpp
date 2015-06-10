#include "RectObject.h"
#include "Material.h"
#include "Shaders.h"

/////RECT OBJECT/////


std::vector<RectObject*> RectObject::rectList(MAX_RECTS, nullptr);
int RectObject::rectCount = 0;


RectObject::RectObject(Point3f p1, Point3f p2, Point3f p3, Material *mat, bool no_shadow)
	: RayObject(RayObjType::RECT),
		material(mat), drawOnly(no_shadow)
{
	points[0] = p1;
	points[1] = p2;
	points[2] = p3;

	auto iter = std::find(rectList.begin(), rectList.end(), nullptr);
	if(iter != rectList.end())
	{
		index = (iter - rectList.begin());
		rectList[index] = this;
		rectCount++;
	}
	else
		std::cout << "TOO MANY RECTS! Increase MAX_RECTS.\n";
}

RectObject::~RectObject()
{
	if(index >= 0 && index < MAX_RECTS)
	{
		rectList.erase(rectList.begin() + index);
		rectList.push_back(nullptr);
		rectCount--;
	}
}

void RectObject::updateShaderList(ShaderProgram *pSP)
{
	for(unsigned int i = 0; i < MAX_RECTS; i++)
	{
		RectObject *r = rectList[i];
		if(r)
			r->updateShader(pSP);
		else
		{
			//pSP->setUniform3f("rectList[" + std::to_string(i) + "].points[0]", 0.0f, 0.0f, 0.0f);
			//pSP->setUniform3f("rectList[" + std::to_string(i) + "].points[1]", 0.0f, 0.0f, 0.0f);
			//pSP->setUniform3f("rectList[" + std::to_string(i) + "].points[2]", 0.0f, 0.0f, 0.0f);
		}
	}

	pSP->setUniform1i("numRects", rectCount);
}

unsigned int RectObject::numRects()
{
	return rectCount;
}

void RectObject::setPoints(Point3f p1, Point3f p2, Point3f p3)
{
	points[0] = p1;
	points[1] = p2;
	points[2] = p3;
}
	
void RectObject::updateShader(ShaderProgram *pSP)
{
	std::string name = "rectList[" + std::to_string(index) + "]";

	float point_values[] = {points[0].x, points[0].y, points[0].z,
							points[1].x, points[1].y, points[1].z,
							points[2].x, points[2].y, points[2].z};

	pSP->setUniform3fv(name + ".points[0]", 3, point_values);
	//pSP->setUniform3f(name + ".points[1]", points[1].x, points[1].y, points[1].z);
	//pSP->setUniform3f(name + ".points[2]", points[2].x, points[2].y, points[2].z);
	pSP->setUniform1i(name + ".material", (material ? material->getIndex() : 0));
	pSP->setUniform1i(name + ".drawOnly", (int)drawOnly);
}