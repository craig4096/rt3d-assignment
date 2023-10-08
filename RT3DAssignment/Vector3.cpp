#include "Vector3.h"
#include <cstdlib>
#include <math.h>

Vector3::Vector3(void)
	: x(0.0), y(0.0), z(0.0)
{
}

Vector3::Vector3(float x, float y, float z)
	: x(x), y(y), z(z)
{
}

Vector3::Vector3(float pos[3])
{
	x = pos[0];
	y = pos[1];
	z = pos[2];
}

Vector3::Vector3(std::ifstream& ifs)
{
	ifs >> x;
	ifs >> y;
	ifs >> z;
}

Vector3::~Vector3(void)
{
}


bool Vector3::Equals(const Vector3& other, float epsilon) const
{
	return abs(other.x - x) < epsilon &&
		abs(other.y - y) < epsilon;
}

Vector3 Vector3::operator -(const Vector3& other) const
{
	return Vector3(x - other.x, y - other.y, z - other.z);
}

Vector3& Vector3::operator +=(const Vector3& other)
{
	x += other.x;
	y += other.y;
	z += other.z;
	return *this;
}

Vector3& Vector3::operator -=(const Vector3& other)
{
	x -= other.x;
	y -= other.y;
	z -= other.z;
	return *this;
}

Vector3 Vector3::operator +(const Vector3& other) const
{
	return Vector3(x + other.x, y + other.y, z + other.z);
}

Vector3 Vector3::operator *(float scalar) const
{
	return Vector3(x * scalar, y * scalar, z * scalar);
}

Vector3& Vector3::operator *=(float scalar)
{
	x *= scalar;
	y *= scalar;
	z *= scalar;
	return *this;
}


float Vector3::DotProduct(const Vector3& other) const
{
	return x * other.x + y * other.y + z * other.z;
}

float Vector3::DistanceTo(const Vector3& other) const
{
	return (other - *this).Magnitude();
}

Vector3 Vector3::CrossProduct(const Vector3& other) const
{
	return Vector3((y * other.z) - (other.y * z),
					(z * other.x) - (other.z * x),
					(x * other.y) - (other.x * y));
}

Vector3 Vector3::operator /(float scalar) const
{
	return Vector3(x / scalar, y / scalar, z / scalar);
}

float Vector3::Magnitude() const
{
	return sqrt(x*x + y*y + z*z);
}


void Vector3::Normalize()
{
	float mag = Magnitude();
	if(mag == 0.0)
	{
		x = y = z = 0.0;
	}
	// reciprocal of magnitude
	float recipMag = 1.0f / mag;

	x *= recipMag;
	y *= recipMag;
	z *= recipMag;
}

Vector3 Vector3::Normalized() const
{
	float mag = Magnitude();
	if(mag == 0.0)
	{
		return Vector3(0.0, 0.0, 0.0);
	}
	// reciprocal of magnitude
	float recipMag = 1.0 / mag;

	return Vector3(x * recipMag,
					y * recipMag,
					z * recipMag);
}
