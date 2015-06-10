#ifndef CAMERA_H
#define CAMERA_H

//#include "GResources.h"
//#include "Geometry.h"
#include "Matrix.h"
#include "Vector.h"

class ShaderProgram;

class Camera 
{
private:
	//Matrices
	Mat4f viewMatrix;
	Mat4f projMatrix;

	//Camera vectors
	Point3f		camPos;	//Location
	Vec3f		eyeDir;	//Direction pointed
	Vec3f		upDir;		//Direction up

	//Projection values
	float cam_near_z;
	float cam_far_z;
	float cam_aspect;
	float cam_fov;

	void updateView();
	void updateProj();

public:
	Camera();
	Camera(Point3f pos, Vec3f eye, Vec3f up);

	~Camera();
	
	void update();
	void setActive(ShaderProgram *pSP);

	Mat4f& getView();
	Mat4f& getProj();

	//Rotates Camera around its axes by a pitch, yaw, and roll (angles)
	void rotate(float pitch, float yaw, float roll);

	//View set functions
	void setPos(Point3f newPos);
	void setCamera(Point3f pos, Vec3f eye, Vec3f up);

	//Sets projection values
	void setProj(float fov, float aspect, float near_z, float far_z);

	void setFOV(float fov);
	void setAspect(float aspect);
	void setNearZ(float near_z);
	void setFarZ(float far_z);

	//View get functions
	Point3f getPos() const;
	Vec3f getEyeDir() const;
	Vec3f getUpDir() const;
	Vec3f getRightDir() const;
	
	//Proj get functions
	float getFOV() const;
	float getAspect() const;
	float getNearZ() const;
	float getFarZ() const;
};

#endif