#pragma once
#include "Character.h"
#include "Sound.h"

class BadGuy : public Character
{
public:
	BadGuy(Level* level, std::ifstream& ifs);
	~BadGuy(void);


	void Update(float timeStep);

	void DoDamage(int damage);

	bool IsDead() const { return health == 0; }

private:

	// the character the enemy is pursuing
	// most probaly the player
	Character* pursuit;

	enum State
	{
		WANDERING,
		ROTATING,
		CONFUSED,
		PURSUIT,
		ATTACKING,
		DEAD
	}
	state;

	enum Animations
	{
		WALKING_ANIM,
		ATTACK_ANIM,
		DYING_ANIM
	};




	Sound* alertSound;
	Sound* damageSound;
	Sound* dyingSound;
	Sound* attackSound;

	float timeAccumulator;

	float timeToChangeDirection;
	float wanderSpeed;
	float alertRadius;
	float attackRadius;
	float pursuitSpeed;
	int attackDamage;
	int health;
};
