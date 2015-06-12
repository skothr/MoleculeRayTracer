#ifndef RAY_OBJECT_H
#define RAY_OBJECT_H

enum class RayObjectType
{
	INVALID = -1,

	SPHERE = 0,
	RECT = 1,
};

typedef RayObjectType RayObjType;

class ShaderProgram;

//Base class for different types of ray objects
//	- Just holds what type of object it is
class RayObject
{
private:
	const RayObjectType type;

protected:


public:
	RayObject(RayObjectType obj_type);
	virtual ~RayObject();

	RayObjectType getType() const;

	//Sends object resources to the given shader.
	virtual void updateShader(ShaderProgram *pSP) = 0;
};


#endif	//RAY_OBJECT_H