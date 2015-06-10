#ifndef APOLLO_INTERPOLATION_H
#define APOLLO_INTERPOLATION_H

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif	//_USE_MATH_DEFINES
#include <math.h>
#include <vector>

#include "Vector.h"
#include "Color.h"


enum class InterpolationType
{
	INVALID = -1,

	LINEAR = 0,
	QUADRATIC,
	CUBIC,
	QUARTIC,
	POLYNOMIAL,
	COSINE,
	SMOOTH,
	SMOOTHER,
	CUBIC_SPLINE,

	SPRING_MOTION,

	MATCH_ADJACENT,
	DATA
};
typedef InterpolationType Interpolation;
typedef InterpolationType Interp;




#define COMPUTE_INTERP_ALPHA(s_x, x0, x1) ((s_x - x0)/(x1 - x0))


/////LINEAR INTERPOLATION/////

//Samples between y0 and y1 by factor a (0 <= a <= 1)
inline double lerp(double y0, double y1, double a)
{
	return y0 + (y1 - y0)*a;
}
//Samples value (y) at s_x -- assumes s_x is between p0.x and p1.x
inline double lerp(double s_x, Point2d p0, Point2d p1)
{
	return lerp(p0.y, p1.y, COMPUTE_INTERP_ALPHA(s_x, p0.x, p1.x));
}

inline Color lerp(const Color &c0, const Color &c1, double a)
{
	return c0 + (c1 - c0)*(float)a;
}


/////COSINE INTERPOLATION/////

//Samples between y0 and y1 by factor a (0 <= a <= 1)
inline double cosInterp(double y0, double y1, double a)
{
	double a2 = (1.0 - cos(a*M_PI))/2.0;
	return lerp(y0, y1, a2);
}
//Samples value (y) at s_x -- assumes s_x is between p0.x and p1.x
inline double cosInterp(double s_x, Point2d p0, Point2d p1)
{
	return cosInterp(p0.y, p1.y, COMPUTE_INTERP_ALPHA(s_x, p0.x, p1.x));
}


/////CUBIC INTERPOLATION/////

//Samples between y1 and y2 by factor a (0 <= a <= 1)
inline double cubeInterp(double y0, double y1, double y2, double y3, double a)
{
	double	a2 = a*a,
			c0 = y3 - y2 - y0 + y1,
			c1 = y0 - y1 - c0,
			c2 = y2 - y0,
			c3 = y1;

	return (c0*a*a2 + c1*a2 + c2*a + c3);
}
//Samples value (y) at s_x -- assumes s_x is between p1.x and p2.x
inline double cubeInterp(double s_x, Point2d p0, Point2d p1, Point2d p2, Point2d p3)
{
	return cubeInterp(p0.y, p1.y, p2.y, p3.y, COMPUTE_INTERP_ALPHA(s_x, p1.x, p2.x));
}

/////POLYNOMIAL INTERPOLATION/////
//TODO: General polynomial interpolation

//Samples between the middle elements of points by factor a (0 <= a <= 1)
inline double polyInterp(int degree, std::vector<double> &y_vals, double a)
{
	if(degree < 0)
		return 0.0;
	if(degree == 0)
		return y_vals[0];
	if(degree == 1)
	{
		//Linear
		return lerp(y_vals[0], y_vals[1], a);
	}
	else if(degree == 2)
	{
		//Quadratic
	}
	else if(degree == 3)
	{
		//Cubic
	}
	else
	{
		//N-degree
	}

	return 0.0;
}
//Samples value (y) at s_x -- assumes s_x is between p1.x and p2.x
inline double polyInterp(double s_x, int degree, std::vector<Point2d> &points)
{
	unsigned int center_low = degree - 1;
	std::vector<double> y_vals(points.size());
	for(unsigned int i = 0; i < y_vals.size(); i++)
		y_vals[i] = points[i].y;
	return polyInterp(degree, y_vals, COMPUTE_INTERP_ALPHA(s_x, points[center_low].x, points[center_low + 1].x));
}


/////SMOOTH INTERPOLATION/////

//Samples between y0 and y1 by factor a (0 <= a <= 1)
inline double smoothInterp(double y0, double y1, double a)
{
	return y0 + (y1 - y0)*(a*a*(3.0 - 2.0*a));
}
//Samples value (y) at s_x -- assumes s_x is between p0.x and p1.x
inline double smoothInterp(double s_x, Point2d p0, Point2d p1)
{
	return smoothInterp(p0.y, p1.y, COMPUTE_INTERP_ALPHA(s_x, p0.x, p1.x));
}


/////SMOOTHER INTERPOLATION/////

//Samples between y0 and y1 by factor a (0 <= a <= 1)
inline double smootherInterp(double y0, double y1, double a)
{
	return y0 + (y1-y0)*(a*a*a*(a*(a*6.0 - 15.0) + 10.0));
}
//Samples value (y) at s_x -- assumes s_x is between p0.x and p1.x
inline double smootherInterp(double s_x, Point2d p0, Point2d p1)
{
	return smootherInterp(p0.y, p1.y, COMPUTE_INTERP_ALPHA(s_x, p0.x, p1.x));
}


/////SPLINE INTERPOLATION/////
//TODO: Spline interpolation

//Samples between y0 and y1 by factor a (0 <= a <= 1)
inline double splineInterp(double y0, double y1, double a)
{
	return 0.0;//y0 + (y1-y0)*(a*a*a*(a*(a*6.0 - 15.0) + 10.0));
}
//Samples value (y) at s_x -- assumes s_x is between p0.x and p1.x
inline double splineInterp(double s_x, Point2d p0, Point2d p1)
{
	return 0.0;//splineInterp(p0.y, p1.y, COMPUTE_INTERP_ALPHA(s_x, p0.x, p1.x));
}




/////GENERALIZED INTERPOLATION/////

inline double interpolate(InterpolationType t, std::vector<double> &y_vals, double a)
{
	int degree = 0;
	if(y_vals.size() < 2)
		return 0.0;	//ERROR

	switch(t)
	{
	case Interp::LINEAR:
		return polyInterp(1, y_vals, a);
	case Interp::QUADRATIC:
		return polyInterp(2, y_vals, a);
	case Interp::CUBIC:
		return polyInterp(3, y_vals, a);
	case Interp::QUARTIC:
		return polyInterp(4, y_vals, a);
	case Interp::POLYNOMIAL:
		//Determine degree based off how many values there are
		return polyInterp(y_vals.size()/2, y_vals, a);
	case Interp::COSINE:
		return cosInterp(y_vals[0], y_vals[1], a);
	case Interp::SMOOTH:
		return smoothInterp(y_vals[0], y_vals[1], a);
	case Interp::SMOOTHER:
		return smootherInterp(y_vals[0], y_vals[1], a);
	case Interp::CUBIC_SPLINE:
		return splineInterp(y_vals[0], y_vals[1], a);
	default:
		return lerp(y_vals[0], y_vals[1], a);
	}
}

inline double interpolate(InterpolationType t, double s_x, std::vector<Point2d> &points)
{
	unsigned int center_low = points.size()/2 - 1;
	std::vector<double> y_vals(points.size());
	for(unsigned int i = 0; i < y_vals.size(); i++)
		y_vals[i] = points[i].y;

	return interpolate(t, y_vals, COMPUTE_INTERP_ALPHA(s_x, points[center_low].x, points[center_low + 1].x));
}



#endif	//APOLLO_INTERPOLATION_H