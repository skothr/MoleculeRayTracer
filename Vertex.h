#ifndef VERTEX_H
#define VERTEX_H

#include "Vector.h"

struct Color;

//Vertex -- used for vertices being sent to shaders
struct Vertex
{
	float x, y, z, w;
	float nx, ny, nz;
	float r, g, b, a;

	Vertex();

	template<typename T>
	Vertex(Vector<T, 3> pos, Color col);
	template<typename T>
	Vertex(Vector<T, 3> pos, Vector<T, 3> norm, Color col);

	template<typename T>
	void setNormal(Vector<T, 3> norm);
	
	Point3f getPos();
};

template<typename T>
Vertex::Vertex(Vector<T, 3> pos, Color col)
	: x((float)pos.x), y((float)pos.y), z((float)pos.z), w(1.0f),
	  nx(0.0f), ny(0.0f), nz(0.0f),
	  r((float)col.r), g((float)col.g), b((float)col.b), a((float)col.a)
{ }

template<typename T>
Vertex::Vertex(Vector<T, 3> pos, Vector<T, 3> norm, Color col)
	: x((float)pos.x), y((float)pos.y), z((float)pos.z), w(1.0f),
	  nx((float)norm.x), ny((float)norm.y), nz((float)norm.z),
	  r((float)col.r), g((float)col.g), b((float)col.b), a((float)col.a)
{ }

template<typename T>
void Vertex::setNormal(Vector<T, 3> norm)
{
	nx = static_cast<float>(norm.x);
	ny = static_cast<float>(norm.y);
	nz = static_cast<float>(norm.z);

	return;
}

//Color -- used for colors
struct Color
{
	float r;
	float g;
	float b;
	float a;

	Color();
	Color(float r, float g, float b, float a = 1.0);

	//Add Colors
	Color& operator+=(const Color &rhs);
	const Color operator+(const Color &other) const;
	
	//Subtract Colors
	Color& operator-=(const Color &rhs);
	const Color operator-(const Color &other) const;

	//Scale whole Color up
	Color& operator*=(const float &rhs);
	const Color operator*(const float &other) const;
	
	//Scale whole Color down
	Color& operator/=(const float &rhs);
	const Color operator/(const float &other) const;

	//Scale each value up individually
	Color& operator*=(const Color &rhs);
	const Color operator*(const Color &other) const;
	
	//Scale each value down individually
	Color& operator/=(const Color &rhs);
	const Color operator/(const Color &other) const;

};

#endif	//VERTEX_H