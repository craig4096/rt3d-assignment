#pragma once
#include "Character.h"

class Player : public Character
{
public:
	Player(std::ifstream& ifs);
	~Player(void);


	void Update(float timeStep);

	void SetLevel(Level* level);

	void KeyPressed(unsigned char c);
	void KeyReleased(unsigned char c);

	void DoDamage(int damage);

	int GetHealth() const { return health; }

	bool IsDead() const { return health == 0; }

	bool DeadForAWhile() { return deadTimeAccumulator >= deadTime; }

private:

	// player attacks
	void Attack();

	// flags determining which direction the player
	// is moving
	bool forward;
	bool backward;
	bool left;
	bool right;

	bool attacking;


	Sound* jumpSound;
	Sound* attackSound;

	enum Animations
	{
		WALK,
		DIE,
		STAND,
		JUMP,
		ATTACK
	};

	// is the player jumping?
	bool jumping;

	// how fast the player rotates around y axis
	float rotateSpeed;

	// how fast the character moves
	float speed;

	float deadTimeAccumulator;
	float deadTime;

	int health;
	int initialHealth;
	int attackDamage;
};
