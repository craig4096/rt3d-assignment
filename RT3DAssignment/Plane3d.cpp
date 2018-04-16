#include "Plane3d.h"

Plane3d::Plane3d(void)
{
}

Plane3d::Plane3d(const Vector3& normal, float dist)
	: normal(normal)
	, dist(dist)
{
}

Plane3d::~Plane3d(void)
{
}

bool Plane3d::IntersectsLine(const Vector3& start, const Vector3& end, float& time) const
{
	// get the distance of start and end from the plane
	float startDist = DistanceTo(start);
	float endDist = DistanceTo(end);

	if(startDist < 0.0)
	{
		if(endDist > 0.0)
		{
			time = -startDist / (endDist - startDist);
			return true;
		}
	}
	else
	{
		if(endDist < 0.0)
		{
			time = startDist / (startDist - endDist);
			return true;
		}
	}
	return false;
}


Vector3 Plane3d::Project(const Vector3& v) const
{
	return v + (normal * -DistanceTo(v));
}

float Plane3d::DistanceTo(const Vector3& v) const
{
	return normal.DotProduct(v) - dist;
}


const Vector3& Plane3d::GetNormal() const
{
	return normal;
}