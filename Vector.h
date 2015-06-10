#ifndef APOLLO_VECTOR_H
#define APOLLO_VECTOR_H

#include <iostream>

template<typename T, int N>
class Vector;

/*
//Instantiation
template class Vector<int, 2>;
template class Vector<int, 3>;
template class Vector<int, 4>;
template class Vector<float, 2>;
template class Vector<float, 3>;
template class Vector<float, 4>;
template class Vector<double, 2>;
template class Vector<double, 3>;
template class Vector<double, 4>;
*/

//Typedefs
typedef Vector<int, 2> Vec2i;
typedef Vector<int, 3> Vec3i;
typedef Vector<int, 4> Vec4i;
typedef Vector<float, 2> Vec2f;
typedef Vector<float, 3> Vec3f;
typedef Vector<float, 4> Vec4f;
typedef Vector<double, 2> Vec2d;
typedef Vector<double, 3> Vec3d;
typedef Vector<double, 4> Vec4d;

typedef Vector<int, 2> Point2i;
typedef Vector<int, 3> Point3i;
typedef Vector<int, 4> Point4i;
typedef Vector<float, 2> Point2f;
typedef Vector<float, 3> Point3f;
typedef Vector<float, 4> Point4f;
typedef Vector<double, 2> Point2d;
typedef Vector<double, 3> Point3d;
typedef Vector<double, 4> Point4d;

//Definition
template<typename T, int N>
class Vector
{
private:

public:
	Vector();
	Vector(T fill);
	Vector(T x_, T y_, T z_ = (T)0, T w_ = (T)0);
	Vector(T *i_data);
	//Copy contructor
	template<typename T2, int N2>
	Vector(const Vector<T2, N2> &other);
	~Vector() = default;

	//DATA
	union
	{
		//For accessing like a point/vector
		struct
		{
			T x, y, z, w;
		};
		//For accessing like a color
		struct
		{
			T r, g, b, a;
		};
		//For accessing base data
		T data[N];
	};

	//TODO: template specialization to make each size Vector have different variables
	/*
	template<int NN> union Vector<T, NN>
	{
		T x, y, z, w;
		T data[NN];
	};
	template<> union Vector<T, 2>
	{
		//For accessing like a point/vector
		T x, y;
		//For accessing base data
		T data[N];
	};
	template<> union Vector<T, 3>
	{
		//For accessing like a point/vector
		T x, y, z;
		//For accessing like a color
		T r, g, b;
		//For accessing base data
		T data[N];
	};
	template<> union Vector<T, 4>
	{
		//For accessing like a point/vector
		T x, y, z, w;
		//For accessing like a color
		T r, g, b, a;
		//For accessing base data
		T data[N];
	};
	*/

	//OVERLOADS

	//Assignment
	template<typename T2, int N2>
	Vector<T, N>& operator=(const Vector<T2, N2> &other);

	//Overload << for printing
	template<typename T, int N>
	friend std::ostream& operator<<(std::ostream &os, const Vector<T, N> &v);
	
	template<typename T, int N2>
	Vector<T, N>& operator=(const Vector<T, N2> &other);

	//Negation
	const Vector<T, N> operator-() const;

	//Multiply Vectors (dot product)
	const T operator*(const Vector<T, N> &other) const;
	
	//Multiply Vectors (cross product) (only defined for 3d vectors --> defined outside class)
	//TODO: figure this out
	//Vector<T, 3>& operator%=(const Vector<T, 3> &rhs);
	//friend const Vector<T, 3> operator%(const Vector<T, 3> &v1, const Vector<T, 3> &v2) const;

	//Add Vectors
	Vector<T, N>& operator+=(const Vector<T, N> &rhs);
	const Vector<T, N> operator+(const Vector<T, N> &other) const;

	//Subtract Vectors
	Vector<T, N>& operator-=(const Vector<T, N> &rhs);
	const Vector<T, N> operator-(const Vector<T, N> &other) const;

	//Scale up
	Vector<T, N>& operator*=(const T &s);
	const Vector<T, N> operator*(const T &s) const;
	//Scale down
	Vector<T, N>& operator/=(const T &s);
	const Vector<T, N> operator/(const T &s) const;

	//OTHER FUNCTIONS

	void loadZeros();
	void loadOnes();
	void loadValue(T val);

	//Get length
	T length() const;
	//Get length squared
	T length2() const;
	//Normalize this vector -- returns length
	T normalize();
};

template<typename T, int N>
Vector<T, N>::Vector()
	: x((T)0), y((T)0), z((T)0), w((T)0)
{ loadZeros(); }

template<typename T, int N>
Vector<T, N>::Vector(T fill)
	: x((T)0), y((T)0), z((T)0), w((T)0)
{ loadValue(fill); }

template<typename T, int N>
Vector<T, N>::Vector(T x_, T y_, T z_, T w_)
	: x((T)0), y((T)0), z((T)0), w((T)0)
{
	data[0] = x_;
	data[1] = y_;
	if(N > 2)
	{
		data[2] = z_;
		if(N > 3)
			data[3] = w_;
	}
}

template<typename T, int N>
Vector<T, N>::Vector(T *i_data)
	: x((T)0), y((T)0), z((T)0), w((T)0)
{
	for(int i = 0; i < N; i++)
		data[i] = i_data[i];
}

//Copy contructor
template<typename T, int N> template<typename T2, int N2>
Vector<T, N>::Vector(const Vector<T2, N2> &other)
//	: data{(T)0}
{
	if(N2 < N)
		loadZeros();
	if(N2 <= N)// && (typeid(T) != typeid(T2) || this != &other))
	{
		for(int i = 0; i < N2; i++)
				data[i] = static_cast<T>(other.data[i]);
	}
	else
	{
		for(int i = 0; i < N; i++)
				data[i] = static_cast<T>(other.data[i]);
	}
}

////OPERATORS

template<typename T, int N> template<typename T2, int N2>
Vector<T, N>& Vector<T, N>::operator=(const Vector<T2, N2> &other)
{
	if(N2 < N)
		loadZeros();

	if(N2 <= N)// && (typeid(T) != typeid(T2) || this != &other))
	{
		for(int i = 0; i < N2; i++)
				data[i] = static_cast<T>(other.data[i]);
	}
	else
	{
		for(int i = 0; i < N; i++)
				data[i] = static_cast<T>(other.data[i]);
	}
}

//Overload << for printing
template<typename T, int N>
std::ostream& operator<<(std::ostream &os, const Vector<T, N> &v)
{
	//Get ostream flags
	std::ios::fmtflags f = os.flags();
	//Set new ostream flags
	os.setf(std::ios::fixed);
	
	os << "< ";
	for(int i = 0; i < N - 1; i++)
		os << v.data[i] << ", ";
	os << v.data[N-1] << " >";
	
	//Reset ostream flags
	os.flags(f);

	return os;
}

//Negation
template<typename T, int N>
const Vector<T, N> Vector<T, N>::operator-() const
{
	Vector<T, N> neg;
	for(int i = 0; i < N; i++)
		neg.data[i] = -data[i];
	return neg;
}

//Multiply Vectors (dot product)
template<typename T, int N>
const T Vector<T, N>::operator*(const Vector<T, N> &other) const
{
	T sum = (T)0;
	for(int i = 0; i < N; i++)
		sum += data[i]*other.data[i];

	return sum;
}

//Multiply Vectors (cross product) (only defined for 3d vectors)
/*
template<typename T>
Vector<T, 3>& Vector<T, 3>::operator%=(const Vector<T, 3> &rhs)
{
	x = y*rhs.z - z*rhs.y;
	y = z*rhs.x - x*rhs.z;
	z = x*rhs.y - y*rhs.x;
	return *this;
}


template<typename T>
const Vector<T, 3> operator%(const Vector<T, 3> &v1, const Vector<T, 3> &v2) const
{
	Vector<T, 3> result;
	result.x = v1.y*v2.z - v1.z*v2.y;
	result.y = v1.z*v2.x - v1.x*v2.z;
	result.z = v1.x*v2.y - v1.y*v2.x;
	return result;
}
*/

//Add Vectors
template<typename T, int N>
Vector<T, N>& Vector<T, N>::operator+=(const Vector<T, N> &rhs)
{
	for(int i = 0; i < N; i++)
		data[i] += rhs.data[i];
	return *this;
}
template<typename T, int N>
const Vector<T, N> Vector<T, N>::operator+(const Vector<T, N> &other) const
{
	Vector<T, N> result = *this;
	return result += other;
}

//Subtract Vectors
template<typename T, int N>
Vector<T, N>& Vector<T, N>::operator-=(const Vector<T, N> &rhs)
{
	for(int i = 0; i < N; i++)
		data[i] -= rhs.data[i];
	return *this;
}
template<typename T, int N>
const Vector<T, N> Vector<T, N>::operator-(const Vector<T, N> &other) const
{
	Vector<T, N> result = *this;
	return result -= other;
}

//Scale up
template<typename T, int N>
Vector<T, N>& Vector<T, N>::operator*=(const T &s)
{
	for(int i = 0; i < N; i++)
		data[i] *= s;
	return *this;
}
template<typename T, int N>
const Vector<T, N> Vector<T, N>::operator*(const T &s) const
{
	Vector<T, N> result = *this;
	return result *= s;
}
//Scale down
template<typename T, int N>
Vector<T, N>& Vector<T, N>::operator/=(const T &s)
{
	for(int i = 0; i < N; i++)
		data[i] /= s;
	return *this;
}
template<typename T, int N>
const Vector<T, N> Vector<T, N>::operator/(const T &s) const
{
	Vector<T, N> result = *this;
	return result /= s;
}


template<typename T, int N>
void Vector<T, N>::loadZeros()
{
	for(int i = 0; i < N; i++)
		data[i] = (T)0;
}

template<typename T, int N>
void Vector<T, N>::loadOnes()
{
	for(int i = 0; i < N; i++)
		data[i] = (T)1;
}

template<typename T, int N>
void Vector<T, N>::loadValue(T val)
{
	for(int i = 0; i < N; i++)
		data[i] = val;
}

//Get length
template<typename T, int N>
T Vector<T, N>::length() const
{
	T sum = (T)0;
	for(int i = 0; i < N; i++)
		sum += data[i]*data[i];
	return sqrt(sum);
}
//Get length squared
template<typename T, int N>
T Vector<T, N>::length2() const
{
	T sum = (T)0;
	for(int i = 0; i < N; i++)
		sum += data[i]*data[i];
	return sum;
}
//Normalize this vector -- returns length
template<typename T, int N>
T Vector<T, N>::normalize()
{
	T l = length();
	for(int i = 0; i < N; i++)
		data[i] /= l;
	return l;
}


//EXTRA VECTOR FUNCTIONS//

//Returns the cross product of the vectors
template<typename T>
Vector<T, 3> VectorCross(Vector<T, 3> v1, Vector<T, 3> v2)
{
	Vector<T, 3> vf;
	vf.x = v1.y*v2.z - v1.z*v2.y;
	vf.y = v1.z*v2.x - v1.x*v2.z;
	vf.z = v1.x*v2.y - v1.y*v2.x;

	return vf;
}

//Rotates Vector v around axis a by theta
template<typename T>
Vector<T, 3> VectorRotateAxis(Vector<T, 3> v, Vector<T, 3> a, T theta)
{
	Vector<T, 3> new_v;

	//Get length of a and dot product of v and a
	T a_det_2 = a.x*a.x + a.y*a.y + a.z*a.z;
	T a_det = sqrt(a_det_2);
	T dot = a.x*v.x + a.y*v.y + a.z*v.z;

	//Some trigonometric values
	T c = cos(theta);
	T s = sin(theta);
	T t = 1 - c;

	//Multiply v by a vector-rotation matrix (found here: http://www.cprogramming.com/tutorial/3d/rotation.html)
	new_v.x = v.x*(t*a.x*a.x + c) + v.y*(t*a.x*a.y + s*a.z) + v.z*(t*a.x*a.y - s*a.y);
	new_v.y = v.x*(t*a.x*a.y - s*a.z) + v.y*(t*a.y*a.y + c) + v.z*(t*a.y*a.z + s*a.x);
	new_v.z = v.x*(t*a.x*a.z + s*a.y) + v.y*(t*a.y*a.z - s*a.x) + v.z*(t*a.z*a.z + c);

	return new_v;
}


#endif	//APOLLO_VECTOR_H