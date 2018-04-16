#include "Coin.h"
#include <GL/freeglut.h>
#include <stdlib.h>
using namespace std;

Coin::Coin(void)
	: collected(false)
	, rotation(0.0)
{
}

Coin::Coin(ifstream& ifs)
	: collected(false)
	, rotation(0.0)
{
	ifs >> position.x;
	ifs >> position.y;
	ifs >> position.z;

	ifs >> radius;
	ifs >> thickness;
	ifs >> rotationSpeed;

	material = Material(ifs);
}

Coin::~Coin(void)
{
}


void Coin::SetPosition(const Vector3& pos)
{
	position = pos;
}


void Coin::Update(float timeStep)
{
	rotation += rotationSpeed * timeStep;
}


void Coin::Collect()
{
	collected = true;
}

bool Coin::IsCollected() const
{
	return collected;
}


void Coin::Draw()
{
	if(!collected)
	{
		glPushMatrix();
		glTranslatef(position.x, position.y, position.z);
		glRotatef(rotation, 0, 1, 0);

		material.Set();
		glutSolidCylinder(radius, thickness, 16, 16);

		glPopMatrix();
	}
}
