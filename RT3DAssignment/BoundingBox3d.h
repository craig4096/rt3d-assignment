#pragma once

#include "Vector3.h"
#include "Plane3d.h"

class BoundingBox3d
{
public:
	BoundingBox3d();
	BoundingBox3d(const Vector3& point);
	~BoundingBox3d(void);


	// if the point lies outside the bounding box this function
	// will expand the bounding box to fit
	void AddPoint(const Vector3& point);

	Vector3 GetCenter() const;

	// gets the six planes defining this bounding box, normals pointing out
	// from the center
	void GetPlanes(Plane3d planes[6]) const;

	// determines whether the specified line intersects with this bounding box
	// returns the point of intersection as a ratio between 0 and 1
	// also returns the plane equation(plane) of the surface that the line intersected with
	bool IntersectsLine(const Vector3& start, const Vector3& end, float& time, Plane3d& plane) const;


	// determines whether this bounding box is intersecting with another
	bool IntersectsOther(const BoundingBox3d& other) const;

	Vector3 min, max;
};
