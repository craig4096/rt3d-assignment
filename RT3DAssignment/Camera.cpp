#include "Camera.h"
#include <GL/freeglut.h>
#include "Level.h"

Camera::Camera(Character* character, std::ifstream& ifs)
	: character(character)
{
	ifs >> cameraRadius;
	ifs >> cameraHeight;
	ifs >> minGroundHeight;
}

Camera::~Camera(void)
{
}


void Camera::Set()
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	Vector3 dst = character->GetPosition();
	gluLookAt(position.x, position.y, position.z, dst.x, dst.y, dst.z, 0, 1, 0);
}


const Vector3& Camera::GetPosition() const
{
	return position;
}


Vector3 Camera::GetDirection() const
{
	return (character->GetPosition() - position).Normalized();
}

void Camera::Update(float dTime)
{
	float dist = (this->position - character->GetPosition()).Magnitude();

	// get the desired position of the camera, i.e.
	// right behind the character
	Vector3 target = character->GetTarget();
	Vector3 charPos = character->GetPosition();


	Vector3 dst(charPos.x + (target.x * -cameraRadius), 0.0f, charPos.z + (target.z * -cameraRadius));

	float height = character->GetLevel()->GetHeightmap()->GetHeight(dst.x, dst.z) + minGroundHeight;

	dst.y = std::max(character->GetPosition().y + cameraHeight, height);

	Vector3 dir(dst - position);
	dir.Normalize();

	position = dst;
}
