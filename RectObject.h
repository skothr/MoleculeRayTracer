#ifndef RECT_OBJECT_H
#define RECT_OBJECT_H

#include "RayObject.h"
#include "Vector.h"

#include <vector>

#define MAX_RECTS 8

class Material;

class RectObject : public RayObject
{
protected:
	//The rect is defined by 3 points
	Point3f points[3];
	bool drawOnly;
	int index = -1;

	Material *material = nullptr;

	static std::vector<RectObject*> rectList;
	static int rectCount;

public:
	RectObject(Point3f p1, Point3f p2, Point3f p3, Material *mat, bool no_shadow = false);
	virtual ~RectObject();
	
	static void updateShaderList(ShaderProgram *pSP);
	static unsigned int numRects();

	void setPoints(Point3f p1, Point3f p2, Point3f p3);
	
	virtual void updateShader(ShaderProgram *pSP) override;
};


#endif	//RECT_OBJECT_H