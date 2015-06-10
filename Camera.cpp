#include <math.h>
#include <iostream>

#include "Camera.h"
#include "Shaders.h"

//Camera definitions

Camera::Camera()
{
	//Set variables to default values
	camPos = Point3f(-2.0, 2.0, -2.0);
	eyeDir = Vec3f(0.0, 0.0, 1.0);
	upDir = Vec3f(0.0, 1.0, 0.0);

	updateView();
}

//Common Camera constructor
Camera::Camera(Point3f pos, Vec3f eye, Vec3f up)
{
	camPos = Point3f(pos);
	eyeDir = Vec3f(eye);
	upDir = Vec3f(up);

	updateView();
}

Camera::~Camera()
{
}

void Camera::update()
{
	updateView();
	updateProj();
}

void Camera::setActive(ShaderProgram *pSP)
{
	//update();

	pSP->setUniformMat4("viewMatrix", viewMatrix);
	pSP->setUniformMat4("projMatrix", projMatrix);
}

Mat4f& Camera::getView()
{
	return viewMatrix;
}

Mat4f& Camera::getProj()
{
	return projMatrix;
}

//Rotates camera around its axes by a pitch and yaw (roll is unneeded with this fps-style camera)
void Camera::rotate(float pitch, float yaw, float roll)
{
	//Get the y-axis
	Vec3f y_axis = Vec3f(0.0f, 1.0f, 0.0f);

	//Apply yaw
	eyeDir = VectorRotateAxis(eyeDir, y_axis, yaw);
	eyeDir.normalize();
	upDir = VectorRotateAxis(upDir, y_axis, yaw);
	upDir.normalize();

	//Get right vector
	Vec3f right = VectorCross(eyeDir, upDir);
	//right.dy = 0.0;
	right.normalize();

	//Apply pitch
	if(eyeDir.y >= 0.99f && pitch < 0)
	{
		eyeDir = Vec3f(0.0f, 1.0f, 0.0f);		//Straight up
	}
	else if(eyeDir.y <= -0.99f && pitch > 0)
	{
		eyeDir = Vec3f(0.0f, -1.0f, 0.0f);	//Straight down
	}
	else
	{
		eyeDir = VectorRotateAxis(eyeDir, right, pitch);
		eyeDir.normalize();
	}
	
	//Recalculate upDir
	upDir = VectorCross(right, eyeDir);
	upDir.normalize();

	//Apply roll
	//upDir = VectorNormalize(VectorRotateAxis(upDir, eyeDir, roll));
	if(upDir.y < 0.99f && abs(eyeDir.y) < 1.0f)
	{
		upDir = Vec3f(0.0f, 1.0f, 0.0f);
	}
}

//Set functions

//Set camera position, eye, and up
void Camera::setCamera(Point3f pos, Vec3f eye, Vec3f up)
{
	camPos = pos;
	eyeDir = eye;
	upDir = up;

	updateView();
	
	return;
}

//Set camera position
void Camera::setPos(Point3f new_pos)
{
	camPos = new_pos;
	
	return;
}

//Sets projection values
void Camera::setProj(float fov, float aspect, float near_z, float far_z)
{
	//Update values
	cam_fov = fov;
	cam_aspect = aspect;
	setNearZ(near_z);
	setFarZ(far_z);

	updateProj();
	
	return;
}

void Camera::setFOV(float fov)
{
	cam_fov = fov;
	
	return;
}

void Camera::setAspect(float aspect)
{
	cam_aspect = aspect;

	return;
}

void Camera::setNearZ(float near_z)
{
	cam_near_z = near_z;

	return;
}

void Camera::setFarZ(float far_z)
{
	cam_far_z = far_z;

	return;
}


//Returns Camera position
Point3f Camera::getPos() const
{
	return camPos;
}

//Returns Direction Camera is pointed
Vec3f Camera::getEyeDir() const
{
	return eyeDir;
}

//Returns Camera's up direction
Vec3f Camera::getUpDir() const
{
	return upDir;
}

Vec3f Camera::getRightDir() const
{
	return VectorCross(eyeDir, upDir);
}


float Camera::getFOV() const
{
	return cam_fov;
}

float Camera::getAspect() const
{
	return cam_aspect;
}

float Camera::getNearZ() const
{
	return cam_near_z;
}

float Camera::getFarZ() const
{
	return cam_far_z;
}

//Updates modelview matrix
void Camera::updateView()
{
	viewMatrix.loadView(camPos, eyeDir, upDir);
}

//Updates projection matrix
void Camera::updateProj()
{
	projMatrix.loadPerspective(cam_fov, cam_aspect, cam_near_z, cam_far_z);
}
