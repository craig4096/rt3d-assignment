#pragma once

#include "Vector3.h"
#include "Heightmap.h"
#include <fstream>
#include "MD2Model.h"
#include "Sound.h"
#include "Material.h"
#include <map>
class Level;

class Character
{
public:
	Character(Level* level, std::ifstream& ifs);
	~Character(void);

	// draws the character
	void Draw();

	// updates the character
	virtual void Update(float timeStep);

	// does a specific amount of damage to this character
	virtual void DoDamage(int damage) = 0;

	// determines whether this character is dead or not
	virtual bool IsDead() const = 0;

	// returns the type of character, characters with different types
	// will attack each other
	const std::string& GetType() const { return type; }

	// returns the target vector of the character i.e.
	// the direction they are facing
	Vector3 GetTarget() const;

	// returns the position of this character
	const Vector3& GetPosition() const { return position; }


	// returns the rotation (degrees) about the y axis that this
	// character will need to be in for a given direction
	// they are facing
	static float GetRotation(const Vector3& dir);


	void AddAnimation(unsigned int index, std::ifstream& ifs);
	void SetAnimation(unsigned int index, bool looping = true);

	Level* GetLevel() { return level; }

protected:

	// the type of character, characters of the same type will not attack each other
	std::string type;

	// did the character collide with the ground
	// in the last frame?
	bool collidedWithGround;

	Vector3 acceleration;
	Vector3 velocity;
	Vector3 position;
	Vector3 scale;
	// the direction the character is facing (rotation around y axis)
	float rotation;

	Level* level;

	// the size of the character (bounding box)
	Vector3 size;

	// the speed of the animations
	float framesPerSecond;
	// the current frame of animation
	float currentFrame;

	// the start and end frames of the current animation
	float startFrame, endFrame;

	// is the current animation looping or not?
	bool looping;



	struct Animation
	{
		// start and end frames
		float start, end, fps;
	};
	std::map<unsigned int, Animation> animations;


	unsigned int currentAnimation;


	// the character model md2
	MD2Model* model;
	Material material;
	unsigned int texture;
};
