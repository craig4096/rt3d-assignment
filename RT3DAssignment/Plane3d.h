#pragma once

#include "Vector3.h"

class Plane3d
{
public:
	Plane3d(void);
	~Plane3d(void);

	Plane3d(const Vector3& normal, float dist);



	// does the specified line intersect with this plane?
	// return the time between 0 and 1 where they intersect
	bool IntersectsLine(const Vector3& start, const Vector3& end, float& time) const;

	// projects a vector onto the plane so that the new vector will
	// be coplanar to this plane
	Vector3 Project(const Vector3& v) const;

	// returns the distance vector v is from this plane surface
	float DistanceTo(const Vector3& v) const;

	const Vector3& GetNormal() const;

	Vector3 normal;
	float dist;
};
