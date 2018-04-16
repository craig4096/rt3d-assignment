#include "BoundingBox3d.h"

BoundingBox3d::BoundingBox3d()
{
}

BoundingBox3d::BoundingBox3d(const Vector3& point)
{
	min = point;
	max = point;
}

BoundingBox3d::~BoundingBox3d(void)
{
}


void BoundingBox3d::AddPoint(const Vector3& p)
{
	if(p.x < min.x) min.x = p.x;
	if(p.y < min.y) min.y = p.y;
	if(p.z < min.z) min.z = p.z;

	if(p.x > max.x) max.x = p.x;
	if(p.y > max.y) max.y = p.y;
	if(p.z > max.z) max.z = p.z;
}

Vector3 BoundingBox3d::GetCenter() const
{
	return (min + max) / 2.0;
}


void BoundingBox3d::GetPlanes(Plane3d planes[6]) const
{
	planes[0] = Plane3d(Vector3(0, 1, 0), max.y);		// top
	planes[1] = Plane3d(Vector3(0, -1, 0), -min.y);		// bottom
	planes[2] = Plane3d(Vector3(1, 0, 0), max.x);		// left
	planes[3] = Plane3d(Vector3(-1, 0, 0), -min.x);		// right
	planes[4] = Plane3d(Vector3(0, 0, 1), max.z);		// front
	planes[5] = Plane3d(Vector3(0, 0, -1), -min.z);		// back
}

bool BoundingBox3d::IntersectsLine(const Vector3& start, const Vector3& end, float& time, Plane3d& plane) const
{
	Plane3d planes[6];
	GetPlanes(planes);

	// need to check for an intersection with the planes of the box that are
	// front facing
	Vector3 view = (end - start);
	view.Normalize();

	for(unsigned int i = 0; i < 6; ++i)
	{
		if(view.DotProduct(planes[i].GetNormal()) < 0.0)
		{
			// plane is front facing, check for an intersection
			if(planes[i].IntersectsLine(start, end, time))
			{
				// get the point of intersection
				Vector3 poi = start + ((end - start) * time);

				// now check that the point of intersection lies
				// within all other planes
				bool inside = true;
				for(unsigned int j = 0; j < 6 && inside; ++j)
				{
					if(i == j) continue;

					if(planes[j].DistanceTo(poi) > 0.0)
					{
						// point lies outside plane
						inside = false;
					}
				}

				// if the point lies inside then this must be a
				// point of intersection
				if(inside)
				{
					plane = planes[i];
					return true;
				}
			}
		}
	}

	return false;
}

bool BoundingBox3d::IntersectsOther(const BoundingBox3d& other) const
{
	return
		min.x < other.max.x &&
		min.y < other.max.y &&
		min.z < other.max.z &&
		max.x > other.min.x &&
		max.y > other.min.y &&
		max.z > other.min.z;
}
