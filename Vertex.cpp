#include "Vertex.h"


//Vertex definitions

Vertex::Vertex()
	: x(0.0f), y(0.0f), z(0.0f), w(1.0f),
	  nx(0.0f), ny(0.0f), nz(0.0f),
	  r(0.0f), g(0.0f), b(0.0f), a(1.0f)
{ }

Point3f Vertex::getPos()
{
	return Point3f(x, y, z);
}


//Color Definitions

Color::Color()
{
	r = 0.0f;
	g = 0.0f;
	b = 0.0f;
	a = 0.0f;
}

Color::Color(float r, float g, float b, float a)
{
	this->r = r;
	this->g = g;
	this->b = b;
	this->a = a;
}


//Add Colors
Color& Color::operator+=(const Color &rhs)
{
	r += rhs.r;
	g += rhs.g;
	b += rhs.b;

	return *this;
}

const Color Color::operator+(const Color &rhs) const
{
	Color result = *this;
	result += rhs;

	return result;
}

	
//Subtract Colors
Color& Color::operator-=(const Color &rhs)
{
	r -= rhs.r;
	g -= rhs.g;
	b -= rhs.b;

	return *this;
}

const Color Color::operator-(const Color &rhs) const
{
	Color result = *this;
	result -= rhs;

	return result;
}


//Scale whole Colors up
Color& Color::operator*=(const float &rhs)
{
	r *= rhs;
	g *= rhs;
	b *= rhs;

	return *this;
}

const Color Color::operator*(const float &rhs) const
{
	Color result = *this;
	result *= rhs;

	return result;
}

	
//Scale whole Colors down
Color& Color::operator/=(const float &rhs)
{
	r /= rhs;
	g /= rhs;
	b /= rhs;

	return *this;
}

const Color Color::operator/(const float &rhs) const
{
	Color result = *this;
	result /= rhs;

	return result;
}


//Scale each value up individually
Color& Color::operator*=(const Color &rhs)
{
	r *= rhs.r;
	g *= rhs.g;
	b *= rhs.b;
	a *= rhs.a;

	return *this;
}

const Color Color::operator*(const Color &rhs) const
{
	Color result = *this;
	result *= rhs;

	return result;
}


//Scale each value down individually
Color& Color::operator/=(const Color &rhs)
{
	r /= rhs.r;
	g /= rhs.g;
	b /= rhs.b;

	return *this;
}

const Color Color::operator/(const Color &rhs) const
{
	Color result = *this;
	result /= rhs;

	return result;
}