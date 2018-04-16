#include "Character.h"
#include <GL/freeglut.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <sstream>
#include "TextureLoader.h"
#include "Game.h"
#include <iostream>
using namespace std;



Character::Character(Level* level, std::ifstream& ifs)
	: level(level)
	, currentFrame(0.0f)
	, startFrame(0.0f)
	, endFrame(0.0f)
	, looping(false)
	, collidedWithGround(false)
{
	ifs >> type;

	std::string modelFile;
	ifs >> modelFile;

	model = MD2Model::Load(modelFile);

	std::string textureFile;
	ifs >> textureFile;

	texture = TextureLoader::LoadTexture(textureFile, true);

	position = Vector3(ifs);
	scale = Vector3(ifs);
	size = Vector3(ifs);
	ifs >> rotation;

	material = Material(ifs);
}

Character::~Character(void)
{
}



Vector3 Character::GetTarget() const
{
	return Vector3(cos(rotation * ((float)M_PI/180.0f)), 0.0f, sin(rotation * ((float)M_PI/180.0f)));
}


void Character::Draw()
{
	material.Set();

	glPushMatrix();
	glTranslatef(position.x, position.y, position.z);
	glScalef(scale.x, scale.y, scale.z);
	glRotatef(rotation, 0, -1, 0);
	glBindTexture(GL_TEXTURE_2D, texture);
	model->SetFrameNumber(currentFrame);
	model->Draw();
	glBindTexture(GL_TEXTURE_2D, -1);
	glPopMatrix();

}

float Character::GetRotation(const Vector3& dir)
{
	float x = dir.z;
	float y = dir.x;

	float ang;

	if(x >= 0)
	{
		if(y >= 0)
		{
			// 1st quadrant
			ang = atan(x / y) * (180.0f / M_PI);
		}
		else
		{
			// 2nd quadrant
			ang = (atan(-y / x) * (180.0f / M_PI)) + 90.0f;
		}
	}
	else
	{
		if(y < 0)
		{
			// 3rd quadrant
			ang = (atan(x / y) * (180.0f / M_PI)) + 180.0f;
		}
		else
		{
			// 4th quadrant
			ang = (atan(y / -x) * (180.0f / M_PI)) + 270.0f;
		}
	}

	return ang;
}

void Character::SetAnimation(unsigned int animation, bool looping)
{
	Animation a = animations[animation];

	startFrame = a.start;
	endFrame = a.end;
	framesPerSecond = a.fps;
	currentFrame = startFrame;
	currentAnimation = animation;
	this->looping = looping;
}

void Character::AddAnimation(unsigned int index, std::ifstream& ifs)
{
	Animation a;
	ifs >> a.start;
	ifs >> a.end;
	ifs >> a.fps;
	animations[index] = a;
}

void Character::Update(float timeStep)
{
	// update the model animation
	currentFrame += framesPerSecond * timeStep;

	if(currentFrame > endFrame)
	{
		if(looping)
		{
			currentFrame = startFrame;
		}
		else
		{
			currentFrame = endFrame;
		}
	}

	// apply gravity
	acceleration += Vector3(0, -9.81, 0);

	velocity += acceleration * timeStep;

	// update position
	Vector3 newPos = position + (velocity * timeStep);

	// character has moved from position to newPos
	// need to check for a collision and slide the character against walls
	// floors etc.
	newPos = level->SlideBox(position, newPos, this->size, collidedWithGround);

	if(collidedWithGround)
	{
		velocity.y = 0.0;
	}

	// set the new position
	position = newPos;

	// set acceleration back to 0
	acceleration = Vector3(0.0f, 0.0f, 0.0f);
}
