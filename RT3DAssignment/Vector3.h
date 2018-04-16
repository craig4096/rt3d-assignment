#pragma once
#include <fstream>

class Vector3
{
public:
	Vector3(void);
	Vector3(std::ifstream& ifs);
	Vector3(float x, float y, float z);
	~Vector3(void);

	Vector3(float pos[3]);

	Vector3& operator +=(const Vector3& other);
	Vector3& operator -=(const Vector3& other);

	Vector3 operator +(const Vector3& other) const;

	Vector3 operator *(float scalar) const;

	Vector3& operator *=(float scalar);

	Vector3 operator /(float scalar) const;

	Vector3 operator -(const Vector3& other) const;


	float DistanceTo(const Vector3& other) const;

	Vector3 CrossProduct(const Vector3& other) const;

	float DotProduct(const Vector3& other) const;

	bool Equals(const Vector3& other, float epsilon) const;

	// returns the magnitude of this vector
	float Magnitude() const;

	// normalizes this vector
	void Normalize();

	// returns a copy of this vector normalized
	Vector3 Normalized() const;


	float x, y, z;
};
