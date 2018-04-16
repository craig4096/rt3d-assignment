#pragma once
#include "Character.h"

class Camera
{
public:

	// camera needs to know about the character in order
	// to orientate itslef towards the player
	Camera(Character* character, std::ifstream& ifs);
	~Camera(void);


	// updates the camera
	void Update(float dTime);


	const Vector3& GetPosition() const;

	// sest the modelview matrix in opengl to the cameras
	// view
	void Set();

	Vector3 GetDirection() const;

private:

	Character* character;
	Vector3 position;


	// the radius around the player where the camera does not move
	// once the character moves out of this radius the camera will
	// start to follow again
	float cameraRadius;

	// how high the camera will be positioned above
	// the player
	float cameraHeight;

	// the minimum height the camera can get from te ground
	float minGroundHeight;
};
