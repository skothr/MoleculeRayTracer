#ifndef MATRIX_H
#define MATRIX_H

#include <vector>

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
#include <math.h>

#include "Vector.h"

//NOTE: Only use this for ints (or other types that can be xored)
#define SWAP(a, b) ((a)^=(b), (b)^=(a), (a)^=(b))

//Prototypes
template<typename T, int N>
class Matrix;
template<typename T, int N>
int numberDigits(const T* data);

/*
//Instantiation
template class Matrix<int, 2>;
template class Matrix<int, 3>;
template class Matrix<int, 4>;
template class Matrix<float, 2>;
template class Matrix<float, 3>;
template class Matrix<float, 4>;
template class Matrix<double, 2>;
template class Matrix<double, 3>;
template class Matrix<double, 4>;
*/

//Typedefs
typedef Matrix<int, 2> Mat2i;
typedef Matrix<int, 3> Mat3i;
typedef Matrix<int, 4> Mat4i;
typedef Matrix<float, 2> Mat2f;
typedef Matrix<float, 3> Mat3f;
typedef Matrix<float, 4> Mat4f;
typedef Matrix<double, 2> Mat2d;
typedef Matrix<double, 3> Mat3d;
typedef Matrix<double, 4> Mat4d;

//Definitions
template<typename T, int N>
class Matrix
{
protected:
	//Multiplication functions
	void mul(const Matrix<T, N> &left, const Matrix<T, N> &right);	//Sets [this] to the product
	void add(const Matrix<T, N> &left, const Matrix<T, N> &right);

	Vector<T, N> mul(const Matrix<T, N> &m, const Vector<T, N> &v) const;
	Vector<T, N> mul(const Vector<T, N> &v, const Matrix<T, N> &m) const;

	void loadValue(T val);

public:
	Matrix();
	Matrix(T fill);
	Matrix(T *i_data, bool row_major);
	//Copy constructor
	template<typename T2, int N2>
	Matrix(const Matrix<T2, N2> &other);
	~Matrix() = default;

	/////DATA

	T data[N][N];	//(row-major --> data[row][col])

	/////OVERLOADS

	//Assignment
	template<typename T2, int N2>
	Matrix<T, N>& operator=(const Matrix<T2, N2> &other);

	//<<
	template<typename T, int N>
	friend std::ostream& operator<<(std::ostream &os, const Matrix<T, N> &m);
	//Negation
	const Matrix<T, N> operator-() const;

	//Multiply this Matrix by another Matrix
	Matrix<T, N>& operator*=(const Matrix<T, N> &rhs);
	const Matrix<T, N> operator*(const Matrix<T, N> &other) const;
	
	//Divide this Matrix by another Matrix
	Matrix<T, N>& operator/=(const Matrix<T, N> &rhs);
	const Matrix<T, N> operator/(const Matrix<T, N> &other) const;
	
	//Add this Matrix to another Matrix
	Matrix<T, N>& operator+=(const Matrix<T, N> &rhs);
	const Matrix<T, N> operator+(const Matrix<T, N> &other) const;

	//Subtract this Matrix from another Matrix
	Matrix<T, N>& operator-=(const Matrix<T, N> &rhs);
	const Matrix<T, N> operator-(const Matrix<T, N> &other) const;
	
	//Multiply Vector by this Matrix
	template<typename T, int N>
	friend const Vector<T, N> operator*(const Matrix<T, N> &m, const Vector<T, N> &v);
	template<typename T, int N>
	friend const Vector<T, N> operator*(const Vector<T, N> &v, const Matrix<T, N> &m);

	////OTHER FUNCTIONS

	void loadZeros();
	void loadIdentity();
	void transpose();

	void loadTranslate(T tx, T ty, T tz = (T)0, T tw = (T)0);
	void loadRotate(T theta_r, Vector<T, N-1> axis);
	void loadScale(T sx, T sy, T sz = (T)1, T sw = (T)1);

	void translate(T tx, T ty, T tz = (T)0, T tw = (T)0);
	void rotate(T theta_r, Vector<T, N-1> axis);
	void scale(T sx, T sy, T sz = (T)1, T sw = (T)1);

	void loadPerspective(float fov, float aspect, float near_z, float far_z);
	void loadView(Vec3f pos, Vec3f eyeDir, Vec3f upDir);

	Matrix<T, N> getInverse();
	void invert();

	Matrix<T, N> getCopy();

	float* toFArray() const;	//ROW MAJOR
	T* toArray() const;			//ROW MAJOR

	void loadArray(T* arr);
};

template<typename T, int N>
Matrix<T, N>::Matrix()
//	: data{(T)0}
{ loadZeros(); }

template<typename T, int N>
Matrix<T, N>::Matrix(T fill)
//	: data{fill}
{ loadValue(fill); }

template<typename T, int N>
Matrix<T, N>::Matrix(T *i_data, bool row_major)
{
	//Store in row-major order either way
	if(row_major)
	{
		for(int r = 0; r < N; r++)
			for(int c = 0; c < N; c++)
					data[r][c] = i_data[r*N + c];
	}
	else
	{
		for(int c = 0; c < N; c++)
			for(int r = 0; r < N; r++)
				data[r][c] = i_data[c*N + r];
	}
}

template<typename T, int N> template<typename T2, int N2>
Matrix<T, N>::Matrix(const Matrix<T2, N2> &other)
	//: data{(T)0}
{
	if(N2 < N)
		loadZeros();

	if(N2 <= N)// && (typeid(T) != typeid(T2) || this != &other))
	{
		for(int r = 0; r < N2; r++)
			for(int c = 0; c < N2; c++)
				data[r][c] = static_cast<T>(other.data[r][c]);
	}
	else
	{
		for(int r = 0; r < N; r++)
			for(int c = 0; c < N; c++)
				data[r][c] = static_cast<T>(other.data[r][c]);
	}
}

template<typename T, int N> template<typename T2, int N2>
Matrix<T, N>& Matrix<T, N>::operator=(const Matrix<T2, N2> &other)
{
	if(N2 < N)
		loadZeros();

	if(N2 <= N)// && (typeid(T) != typeid(T2) || this != &other))
	{
		for(int r = 0; r < N2; r++)
			for(int c = 0; c < N2; c++)
				data[r][c] = static_cast<T>(other.data[r][c]);
	}
	else
	{
		for(int r = 0; r < N; r++)
			for(int c = 0; c < N; c++)
				data[r][c] = static_cast<T>(other.data[r][c]);
	}

	return *this;
}

//Printing
template<typename T, int N>
std::ostream& operator<<(std::ostream &os, const Matrix<T, N> &m)
{
	//Get ostream params
	std::ios::fmtflags f = os.flags();
	std::streamsize p = os.precision();

	//Set new ostream flags
	os.setf(std::ios::fixed | std::ios::right);

	//Get number of digits before decimal
	int digits[N];
	T *d = m.toArray();
	for(int i = 0; i < N; i++)
		digits[i] = numberDigits<T, N>(&d[i*N]);
	delete[] d;

	//Output matrix
	os << "\n";
	for(int r = 0; r < N; r++)
	{
		os << "\t";
		for(int c = 0; c < N; c++)
		{
			os.width((std::streamsize)(p + 3 + digits[c]));
			os << m.data[r][c] << " ";
		}
		os << "\n";
	}
	os << "\n";

	//Reset ostream flags
	os.flags(f);

	return os;
}

//Negation
template<typename T, int N>
const Matrix<T, N> Matrix<T, N>::operator-() const
{
	Matrix<T, N> neg;
	for(int r = 0; r < N; r++)
		for(int c = 0; c < N; c++)
			neg.data[r][c] = -data[r][c];
	return neg;
}

//Multiplication
template<typename T, int N>
Matrix<T, N>& Matrix<T, N>::operator*=(const Matrix<T, N> &rhs)
{
	mul(*this, rhs);
	return *this;
}
template<typename T, int N>
const Matrix<T, N> Matrix<T, N>::operator*(const Matrix<T, N> &other) const
{
	Matrix<T, N> result = *this;
	return result *= other;
}

//Division
template<typename T, int N>
Matrix<T, N>& Matrix<T, N>::operator/=(const Matrix<T, N> &rhs)
{
	mul(rhs, *this);
	return *this;
}
template<typename T, int N>
const Matrix<T, N> Matrix<T, N>::operator/(const Matrix<T, N> &other) const
{
	Matrix<T, N> result = *this;
	return result /= other;
}

//Addition
template<typename T, int N>
Matrix<T, N>& Matrix<T, N>::operator+=(const Matrix<T, N> &rhs)
{
	add(*this, rhs);
	return *this;
}
template<typename T, int N>
const Matrix<T, N> Matrix<T, N>::operator+(const Matrix<T, N> &other) const
{
	Matrix<T, N> result = *this;
	return result += other;
}

//Subtraction
template<typename T, int N>
Matrix<T, N>& Matrix<T, N>::operator-=(const Matrix<T, N> &rhs)
{
	add(*this, -rhs);
	return *this;
}
template<typename T, int N>
const Matrix<T, N> Matrix<T, N>::operator-(const Matrix<T, N> &other) const
{
	Matrix<T, N> result = *this;
	return result -= other;
}

//Multiply Vector by this Matrix
template<typename T, int N>
const Vector<T, N> operator*(const Matrix<T, N> &m, const Vector<T, N> &v)
{
	Vector<T, N> result = m.mul(m, v);
	return result;
}

template<typename T, int N>
const Vector<T, N> operator*(const Vector<T, N> &v, const Matrix<T, N> &m)
{
	Vector<T, N> result = m.mul(v, m);
	return result;
}


//Clear matrix to all zeros
template<typename T, int N>
void Matrix<T, N>::loadZeros()
{
	for(int r = 0; r < N; r++)
		for(int c = 0; c < N; c++)
			data[r][c] = (T)0;
}

//Load an identity matrix into data
template<typename T, int N>
void Matrix<T, N>::loadIdentity()
{
	for(int r = 0; r < N; r++)
		for(int c = 0; c < N; c++)
			data[r][c] = (r == c) ? (T)1 : (T)0;
}

template<typename T, int N>
void Matrix<T, N>::loadValue(T val)
{
	for(int r = 0; r < N; r++)
		for(int c = 0; c < N; c++)
			data[r][c] = val;
}

template<typename T, int N>
void Matrix<T, N>::transpose()
{
	T temp;
	for(int r = 0; r < N; r++)
	{
		for(int c = 0; c < r; c++)
		{
			temp = data[r][c];
			data[r][c] = data[c][r];
			data[c][r] = temp;
			//SWAP(data[r][c], data[c][r]);
		}
	}
}

template<typename T, int N>
void Matrix<T, N>::loadTranslate(T tx, T ty, T tz, T tw)
{
	//Set Matrix
	loadIdentity();
	data[0][N-1] = tx;
	data[1][N-1] = ty;
	if(N > 3)
	{
		data[2][N-1] = tz;
		if(N > 4)
			data[3][N-1] = tw;
	}
}

template<typename T, int N>
void Matrix<T, N>::loadRotate(T theta_r, Vector<T, N - 1> axis)
{
	if(N == 3)
	{
		//2D rotation
	}
	else if(N == 4)
	{
		//3D rotation
		//Vector3<T> axis = Vector3<T>(vx, vy, vz)/sqrt(vx*vx + vy*vy + vz*vz);
		axis.normalize();
		Vector<T, 3> axis_2(axis.x*axis.x, axis.y*axis.y, axis.z*axis.z);

		T cos_t = cos(theta_r);
		T sin_t = sin(theta_r);

		//Set Matrix
		loadIdentity();


		data[0][0] = axis_2.x + ((T)1 - axis_2.x)*cos_t;
		data[1][1] = axis_2.y + ((T)1 - axis_2.y)*cos_t;
		data[2][2] = axis_2.z + ((T)1 - axis_2.z)*cos_t;

	
		data[0][1] = axis.x*axis.y*((T)1 - cos_t) - axis.z*sin_t;
		data[1][0] = axis.x*axis.y*((T)1 - cos_t) + axis.z*sin_t;
	
		data[0][2] = axis.x*axis.z*((T)1 - cos_t) + axis.y*sin_t;
		data[2][0] = axis.x*axis.z*((T)1 - cos_t) - axis.y*sin_t;
	
		data[1][2] = axis.y*axis.z*((T)1 - cos_t) - axis.x*sin_t;
		data[2][1] = axis.y*axis.z*((T)1 - cos_t) + axis.x*sin_t;
	}
	//Else, just leave it how it was
}

template<typename T, int N>
void Matrix<T, N>::loadScale(T sx, T sy, T sz, T sw)
{
	loadIdentity();

	data[0][0] = sx;
	data[1][1] = sy;
	if(N > 3)
	{
		data[2][2] = sz;
		if(N > 4)
			data[3][3] = sw;
	}
}

template<typename T, int N>
void Matrix<T, N>::translate(T tx, T ty, T tz, T tw)
{
	Matrix<T, N> temp;
	temp.loadTranslate(tx, ty, tz, tw);
	mul(temp, *this);
}

template<typename T, int N>
void Matrix<T, N>::rotate(T theta_r, Vector<T, N - 1> axis)
{
	Matrix<T, N> temp;
	temp.loadRotate(theta_r, axis);
	mul(temp, *this);
}

template<typename T, int N>
void Matrix<T, N>::scale(T sx, T sy, T sz, T sw)
{
	Matrix<T, N> temp;
	temp.loadScale(sx, sy, sz, sw);
	mul(temp, *this);
}


template<typename T, int N>
void Matrix<T, N>::loadPerspective(float fov_d, float aspect, float near_z, float far_z)
{
	if(N == 4)
	{
		float	fov_r = fov_d*(T)M_PI/(T)180.0f,
			
				xymax = near_z*tan(fov_r/2),
				ymin = -xymax,
				xmin = -xymax,
				
				width = xymax - xmin,
				height = xymax - ymin,
				
				depth = far_z - near_z,
				q = -(far_z + near_z)/depth,
				qn = -2*(far_z*near_z)/depth,
				
				w = 2*near_z/width/aspect,
				h = 2*near_z/height;

		//Set Matrix
		loadZeros();
		data[0][0] = w;
		data[1][1] = h;
		data[2][2] = q;
		data[2][3] = -1;
		data[3][2] = qn;
	}
	else
		loadIdentity();
		
}

template<typename T, int N>
void Matrix<T, N>::loadView(Point3f pos, Vec3f eye_dir, Vec3f up_dir)
{
	loadIdentity();

	if(N == 4)
	{
		Vec3f z_axis = -eye_dir;
		z_axis.normalize();
		Vec3f x_axis = VectorCross(up_dir, z_axis);
		x_axis.normalize();
		Vec3f y_axis = VectorCross(z_axis, x_axis);
	
		//Set Matrix
		data[0][0] = x_axis.x;						data[0][1] = y_axis.x;						data[0][2] = z_axis.x;
		data[1][0] = x_axis.y;						data[1][1] = y_axis.y;						data[1][2] = z_axis.y;
		data[2][0] = x_axis.z;						data[2][1] = y_axis.z;						data[2][2] = z_axis.z;
		data[3][0] = -(x_axis*pos);					data[3][1] = -(y_axis*pos);					data[3][2] = -(z_axis*pos);
		
	}
}




template<typename T, int N>
Matrix<T, N> Matrix<T, N>::getInverse()
{
	//Load A into new Matrix
	Matrix<T, N> inv = *this;
	inv.invert();

	return inv;
}

template<typename T, int N>
void Matrix<T, N>::invert()
{
	//Make an array 4 rows x 8 columns
	T A[N][N*2];

	for(int r = 0; r < N; r++)
	{
		//Load data into left half of A
		for(int c = 0; c < N; c++)
			A[r][c] = data[r][c];
		
		//Load identity into right half of A
		for(int c = N; c < 2*N; c++)
		{
			if(c - N == r)
				A[r][c] = (T)1;
			else
				A[r][c] = (T)0;
		}
	}

	T mult;
	//Do gaussian-jordan elimination on A
	for(int norm = 0; norm < N; norm++)
	{
		for(int r = 0; r < N; r++)
		{
			if(r != norm)
			{
				mult = A[r][norm] / A[norm][norm];
				for(int c = norm; c < 2*N; c++)
					A[r][c] -= A[norm][c] * mult;
			}
		}
	}
	
	//Normalize diagonal elements
	for(int r = 0; r < N; r++)
	{
		mult = A[r][r];
		for(int c = r; c < 2*N; c++)
			A[r][c] = A[r][c]/mult;
	}
	
	//Load A as new matrix
	for(int r = 0; r < N; r++)
		for(int c = 0; c < N; c++)
			data[r][c] = A[r][c + N];
}


template<typename T, int N>
void Matrix<T, N>::mul(const Matrix<T, N> &left, const Matrix<T, N> &right)
{
	//Need to keep temp variable in case one of the matrices is this one
	Matrix<T, N> temp;
	for(int r = 0; r < N; r++)
		for(int c = 0; c < N; c++)
			for(int p = 0; p < N; p++)
				temp.data[r][c] += left.data[r][p] * right.data[p][c];

	*this = temp;
}

template<typename T, int N>
void Matrix<T, N>::add(const Matrix<T, N> &left, const Matrix<T, N> &right)
{
	for(int r = 0; r < N; r++)
		for(int c = 0; c < N; c++)
			data[r][c] = left.data[r][c] + right.data[r][c];
}

template<typename T, int N>
Vector<T, N> Matrix<T, N>::mul(const Matrix<T, N> &m, const Vector<T, N> &v) const
{
	Vector<T, N> result;
	for(int r = 0; r < N; r++)
		for(int c = 0; c < N; c++)
			result.data[r] += v.data[c] * m.data[r][c];

	return result;
}

//TODO: Check this!
template<typename T, int N>
Vector<T, N> Matrix<T, N>::mul(const Vector<T, N> &v, const Matrix<T, N> &m) const
{
	Vector<T, N> result;
	for(int c = 0; c < N; c++)
		for(int r = 0; r < N; r++)
			result.data[c] += v.data[r] * m.data[r][c];

	return result;
}


template<typename T, int N>
Matrix<T, N> Matrix<T, N>::getCopy()
{
	Matrix<T, N> copy;

	for(int r = 0; r < N; r++)
		for(int c = 0; c < N; c++)
			copy.data[r][c] = data[r][c];

	return copy;
}

//Returns the current matrix as a dynamically allocated float array
template<typename T, int N>
float* Matrix<T, N>::toFArray() const
{
	float *arr = new float[N*N];

	for(int c = 0; c < N; c++)
		for(int r = 0; r < N; r++)
			arr[r*N + c] = static_cast<float>(data[r][c]);

	return arr;
}

//Returns the current matrix as a dynamically allocated T array
template<typename T, int N>
T* Matrix<T, N>::toArray() const
{
	T *arr = new T[N*N];

	for(int r = 0; r < N; r++)
		for(int c = 0; c < N; c++)
			arr[c*N + r] = data[r][c];

	return arr;
}

//Loads arr (column-major) into the current matrix
template<typename T, int N>
void Matrix<T, N>::loadArray(T* arr)
{
	for(int r = 0; r < N; r++)
		for(int c = 0; c < N; c++)
			data[r][c] = arr[r*N + c];
}



//Returns the maximum number of digits before the decimal place in the given data (size N)
template<typename T, int N>
int numberDigits(const T* data)
{
	T max = (T)0;

	for(int i = 0; i < N; i++)
		if(data[i] > max)
			max = data[i];

	return static_cast<int>(ceil(log10(max)));
}





/*


//////OLD MATRIX4 IMPLEMENTATION


template<typename T>
class Matrix4
{
private:
	static std::vector<Matrix4<T>> matStack;
	static int N;
	
	//Multiplication functions
	void mul(Matrix4<T> left, Matrix4<T> right);	//Sets [this] to the product
	Vector4<T> mul(Matrix4<T> left, Vector4<T> right) const;
	Point4<T> mul(Matrix4<T> left, Point4<T> right) const;

	//Helper function -- returns maximum number of digits in data (for printing)
	//int numberDigits(T* data, int num);

public:
	//union
	//{
		T data[4][4];	//data[row][column]
	//	Vec3f[4];		//rows
	//}

	Matrix4();
	Matrix4(const Matrix4& src);
	Matrix4& operator=(const Matrix4& src);
	
	//Overload << for printing
	template<typename T>
	friend std::ostream& operator<<(std::ostream &os, const Matrix4<T> &p);

	//Multiply this Matrix by another Matrix
	Matrix4<T>& operator*=(const Matrix4<T> &rhs);
	const Matrix4<T> operator*(const Matrix4<T> &other) const;
	
	//Multiply Vector by this Matrix
	template<typename T>
	friend Vector4<T> operator*(const Matrix4<T> &m, const Vector4<T> &v);
	template<typename T>
	friend Vector4<T> operator*(const Vector4<T> &v, const Matrix4<T> &m);

	//Multiply Point by this Matrix
	template<typename T>
	friend Point4<T> operator*(const Matrix4<T> &m, const Point4<T> &p);
	template<typename T>
	friend Point4<T> operator*(const Point4<T> &p, const Matrix4<T> &m);

	void zero();
	void loadIdentity();
	void transpose();

	void loadPerspective(T fov, T aspect, T near_z, T far_z);
	void loadView(Point3<T> pos, Vector3<T> eyeDir, Vector3<T> upDir);

	void loadTranslate(T tx, T ty, T tz);
	void loadRotate(T theta_r, T vx, T vy, T vz);
	void loadScale(T sx, T sy, T sz);
	
	void translate(T tx, T ty, T tz);
	void rotate(T theta_r, T vx, T vy, T vz);
	void scale(T sx, T sy, T sz);

	Matrix4 inverse();

	Matrix4<T> getCopy();

	void push();
	void pop();

	float* toFArray() const;
	T* toArray() const;

	void loadArray(T* arr);
};


typedef Matrix4<float>	Mat4f;
typedef Matrix4<double> Mat4d;


/////////////////////////////////////
//Matrix4 Definitions
/////////////////////////////////////

//Static variables

template<typename T> std::vector<Matrix4<T>> Matrix4<T>::matStack;
template<typename T> int Matrix4<T>::N = 4;


template<typename T>
Matrix4<T>::Matrix4()
{
	zero();
}

template<typename T>
Matrix4<T>::Matrix4(const Matrix4<T>& src)
{
	for(int i = 0; i < N; i++)
	{
		for(int j = 0; j < N; j++)
		{
			data[i][j] = src.data[i][j];
		}
	}
}

template<typename T>
Matrix4<T>& Matrix4<T>::operator=(const Matrix4& src)
{
	for(int i = 0; i < N; i++)
	{
		for(int j = 0; j < N; j++)
		{
			data[i][j] = src.data[i][j];
		}
	}

	return *this;
}

template<typename T>
int numberDigits(T* data, int num)
{
	T max = (T)0;

	for(int i = 0; i < num; i++)
		if(data[i] > max)
			max = data[i];

	return static_cast<int>(ceil(log10(max)));
}

//Overload << for printing
template<typename T>
std::ostream& operator<<(std::ostream &os, const Matrix4<T> &m)
{
	//Get ostream params
	std::ios::fmtflags f = os.flags();
	std::streamsize p = os.precision();

	//Set new ostream flags
	os.setf(std::ios::fixed | std::ios::right);

	//Get number of digits before decimal
	T* d = m.toArray();
	int digits[4] = {numberDigits(&d[0], 4),
					 numberDigits(&d[4], 4),
					 numberDigits(&d[8], 4),
					 numberDigits(&d[12], 4)};

	delete[] d;

	//Output matrix
	os << "\n";
	for(int row = 0; row < m.N; row++)
	{
		os << "\t";
		for(int col = 0; col < m.N; col++)
		{
			os.width((std::streamsize)(p + 3 + digits[col]));
			os << m.data[row][col] << " ";
		}
		os << "\n";
	}
	os << "\n";

	//Reset ostream flags
	os.flags(f);

	return os;
}

//Multiply this Matrix by another Matrix
template<typename T>
Matrix4<T>& Matrix4<T>::operator*=(const Matrix4<T> &rhs)
{
	mul(*this, rhs);

	return *this;
}
template<typename T>
const Matrix4<T> Matrix4<T>::operator*(const Matrix4<T> &other) const
{
	Matrix4<T> result = *this;
	result *= other;

	return result;
}

//Multiply Vector by this Matrix
template<typename T>
Vector4<T> operator*(const Matrix4<T> &m, const Vector4<T> &v)
{
	return m.mul(m, v);
}
template<typename T>
Vector4<T> operator*(const Vector4<T> &v, const Matrix4<T> &m)
{
	Matrix4<T> temp = m;
	temp.transpose();

	return temp.mul(temp, v);
}

//Multiply Point by this Matrix
template<typename T>
Point4<T> operator*(const Matrix4<T> &m, const Point4<T> &p)
{
	return m.mul(m, p);
}

template<typename T>
Point4<T> operator*(const Point4<T> &p, const Matrix4<T> &m)
{
	Matrix4<T> temp;
	temp.transpose();

	return temp.mul(temp, p);
}


//Clear matrix to all zeros
template<typename T>
void Matrix4<T>::zero()
{
	for(int x = 0; x < N; x++)
	{
		for(int y = 0; y < N; y++)
		{
			data[x][y] = (T)0;
		}
	}

	return;
}

//Load an identity matrix into data
template<typename T>
void Matrix4<T>::loadIdentity()
{
	zero();
	for(int norm = 0; norm < N; norm++)
	{
		data[norm][norm] = (T)1;
	}

	return;
}

template<typename T>
void Matrix4<T>::transpose()
{
	T temp;
	for(int norm = 0; norm < N; norm++)
	{
		for(int i = 0; i < norm; i++)
		{
			temp = data[norm][i];
			data[norm][i] = data[i][norm];
			data[i][norm] = temp;
		}
	}

	return;
}

template<typename T>
void Matrix4<T>::loadPerspective(T fovy_d, T aspect, T near_z, T far_z)
{
	T fov_r = fovy_d*(T)M_PI/(T)180;

	T xymax = near_z*tan(fov_r/2);
	T ymin = -xymax;
	T xmin = -xymax;

	T width = xymax - xmin;
	T height = xymax - ymin;

	T depth = far_z - near_z;
	T q = -(far_z + near_z)/depth;
	T qn = -2*(far_z*near_z)/depth;

	T w = 2*near_z/width/aspect;
	T h = 2*near_z/height;

	//Set Matrix
	zero();
	data[0][0] = w;
	data[1][1] = h;
	data[2][2] = q;
	data[2][3] = -1;
	data[3][2] = qn;

	return;
}

template<typename T>
void Matrix4<T>::loadView(Point3<T> pos, Vector3<T> eyeDir, Vector3<T> upDir)
{
	Vector3<T> z_axis = VectorNormalize(-eyeDir);
	Vector3<T> x_axis = VectorNormalize(VectorCross(upDir, z_axis));
	Vector3<T> y_axis = VectorCross(z_axis, x_axis);
	
	//Set Matrix
	loadIdentity();
	data[0][0] = x_axis.x;						data[0][1] = y_axis.x;						data[0][2] = z_axis.x;
	data[1][0] = x_axis.y;						data[1][1] = y_axis.y;						data[1][2] = z_axis.y;
	data[2][0] = x_axis.z;						data[2][1] = y_axis.z;						data[2][2] = z_axis.z;
	data[3][0] = -VectorPointDot(x_axis, pos);	data[3][1] = -VectorPointDot(y_axis, pos);	data[3][2] = -VectorPointDot(z_axis, pos);

	return;
}

template<typename T>
void Matrix4<T>::loadTranslate(T tx, T ty, T tz)
{
	//Set Matrix
	loadIdentity();
	data[3][0] = tx;
	data[3][1] = ty;
	data[3][2] = tz;

	return;
}

template<typename T>
void Matrix4<T>::loadRotate(T theta_r, T vx, T vy, T vz)
{
	Vector3<T> axis = Vector3<T>(vx, vy, vz)/sqrt(vx*vx + vy*vy + vz*vz);
	Vector3<T> axis_2 = Vector3<T>(axis.x*axis.x, axis.y*axis.y, axis.z*axis.z);

	T cos_t = cos(theta_r);
	T sin_t = sin(theta_r);

	//Set Matrix
	loadIdentity();


	data[0][0] = axis_2.x + ((T)1 - axis_2.x)*cos_t;
	data[1][1] = axis_2.y + ((T)1 - axis_2.y)*cos_t;
	data[2][2] = axis_2.z + ((T)1 - axis_2.z)*cos_t;

	
	data[0][1] = axis.x*axis.y*((T)1 - cos_t) - axis.z*sin_t;
	data[1][0] = axis.x*axis.y*((T)1 - cos_t) + axis.z*sin_t;
	
	data[0][2] = axis.x*axis.z*((T)1 - cos_t) + axis.y*sin_t;
	data[2][0] = axis.x*axis.z*((T)1 - cos_t) - axis.y*sin_t;
	
	data[1][2] = axis.y*axis.z*((T)1 - cos_t) - axis.x*sin_t;
	data[2][1] = axis.y*axis.z*((T)1 - cos_t) + axis.x*sin_t;

	return;
}

template<typename T>
void Matrix4<T>::loadScale(T sx, T sy, T sz)
{
	zero();

	data[0][0] = sx;
	data[1][1] = sy;
	data[2][2] = sz;
	data[3][3] = (T)1;

	return;
}

template<typename T>
void Matrix4<T>::translate(T tx, T ty, T tz)
{
	Matrix4<T> temp;
	temp.loadTranslate(tx, ty, tz);
	mul(temp, *this);

	return;
}

template<typename T>
void Matrix4<T>::rotate(T theta_r, T vx, T vy, T vz)
{
	Matrix4<T> temp;
	temp.loadRotate(theta_r, vx, vy, vz);
	mul(temp, *this);

	return;
}

template<typename T>
void Matrix4<T>::scale(T sx, T sy, T sz)
{
	Matrix4<T> temp;
	temp.loadScale(sx, sy, sz);
	mul(temp, *this);

	return;
}

template<typename T>
Matrix4<T> Matrix4<T>::inverse()
{
	//Make an array 4 rows x 8 columns
	T A[4][8];

	for(int row = 0; row < N; row++)
	{
		//Load data into left half of A
		for(int col = 0; col < N; col++)
		{
			A[row][col] = data[row][col];
		}
		
		//Load identity into right half of A
		for(int col = N; col < 2*N; col++)
		{
			if(col - N == row)
				A[row][col] = 1.0;
			else
				A[row][col] = (T)0.0;
		}
	}

	T mult;
	//Do gaussian-jordan elimination on A
	for(int norm = 0; norm < N; norm++)
	{
		for(int row = 0; row < N; row++)
		{
			if(row != norm)
			{
				mult = A[row][norm] / A[norm][norm];
				for(int col = norm; col < 2*N; col++)
				{
					A[row][col] -= A[norm][col] * mult;
				}
			}
		}
	}
	
	//Normalize diagonal elements
	for(int row = 0; row < N; row++)
	{
		mult = A[row][row];
		for(int col = row; col < 2*N; col++)
		{
			A[row][col] = A[row][col]/mult;
		}
	}
	
	//Load A into new Matrix
	Matrix4<T> inv;
	for(int row = 0; row < N; row++)
	{
		for(int col = 0; col < N; col++)
		{
			inv.data[row][col] = A[row][col + N];
		}
	}

	return inv;
}


template<typename T>
void Matrix4<T>::mul(Matrix4<T> left, Matrix4<T> right)
{
	Matrix4<T> temp;
	for(int row = 0; row < N; row++)
	{
		for(int col = 0; col < N; col++)
		{
			for(int p = 0; p < N; p++)
			{
				temp.data[row][col] += left.data[row][p] * right.data[p][col];
			}
		}
	}
	*this = temp;

	return;
}

template<typename T>
Vector4<T> Matrix4<T>::mul(Matrix4<T> left, Vector4<T> right) const
{
	Vector4<T> temp;
	
	temp.x = data[0][0]*right.x + data[0][1]*right.y + data[0][2]*right.z + data[0][3]*right.w;
	temp.y = data[1][0]*right.x + data[1][1]*right.y + data[1][2]*right.z + data[1][3]*right.w;
	temp.z = data[2][0]*right.x + data[2][1]*right.y + data[2][2]*right.z + data[2][3]*right.w;
	temp.w = data[3][0]*right.x + data[3][1]*right.y + data[3][2]*right.z + data[3][3]*right.w;

	return temp;
}

template<typename T>
Point4<T> Matrix4<T>::mul(Matrix4<T> left, Point4<T> right) const
{
	Point4<T> temp;

	temp.x = data[0][0]*right.x + data[0][1]*right.y + data[0][2]*right.z + data[0][3]*right.w;
	temp.y = data[1][0]*right.x + data[1][1]*right.y + data[1][2]*right.z + data[1][3]*right.w;
	temp.z = data[2][0]*right.x + data[2][1]*right.y + data[2][2]*right.z + data[2][3]*right.w;
	temp.w = data[3][0]*right.x + data[3][1]*right.y + data[3][2]*right.z + data[3][3]*right.w;

	return temp;
}


template<typename T>
Matrix4<T> Matrix4<T>::getCopy()
{
	Matrix4<T> copy;

	for(int x = 0; x < N; x++)
	{
		for(int y = 0; y < N; y++)
		{
			copy.data[x][y] = data[x][y];
		}
	}

	return copy;
}

//Pushes current matrix onto the stack
template<typename T>
void Matrix4<T>::push()
{
	matStack.push_back(getCopy());

	return;
}

//Pops last matrix on the stack into current matrix
template<typename T>
void Matrix4<T>::pop()
{
	if(matStack.size() < 1)
	{
		loadIdentity();
	}
	else
	{
		//Get last element
		Matrix4<T> popMat = matStack.back();
		//Copy data
		for(int x = 0; x < N; x++)
		{
			for(int y = 0; y < N; y++)
			{
				data[x][y] = popMat.data[x][y];
			}
		}
		//Remove last element
		matStack.pop_back();
	}
	
	return;
}

//Returns the current matrix as a dynamically allocated float array
template<typename T>
float* Matrix4<T>::toFArray() const
{
	float *arr = new float[N*N];

	for(int col = 0; col < N; col++)
	{
		for(int row = 0; row < N; row++)
		{
			arr[row*N + col] = static_cast<float>(data[row][col]);
		}
	}

	return arr;
}

//Returns the current matrix as a dynamically allocated T array
template<typename T>
T* Matrix4<T>::toArray() const
{
	T *arr = new T[N*N];

	for(int row = 0; row < N; row++)
	{
		for(int col = 0; col < N; col++)
		{
			arr[col*N + row] = data[row][col];
		}
	}

	return arr;
}

//Loads arr (column-major) into the current matrix
template<typename T>
void Matrix4<T>::loadArray(T* arr)
{
	for(int i = 0; i < N; i++)
	{
		for(int j = 0; j < N; j++)
		{
			data[i][j] = arr[i*N + j];
		}
	}

	return;
}

*/

#endif	//MATRIX_H