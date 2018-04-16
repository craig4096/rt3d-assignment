#include "BadGuy.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <string>
#include "Level.h"
#include <stdlib.h>
using namespace std;


BadGuy::BadGuy(Level* level, std::ifstream& ifs)
	: Character(level, ifs)
	, state(ROTATING)
	, timeAccumulator(0.0f)
	, pursuit(0)
{
	alertSound = new Sound(ifs, false);
	damageSound = new Sound(ifs, false);
	dyingSound = new Sound(ifs, false);
	attackSound = new Sound(ifs, false);

	ifs >> alertRadius;
	ifs >> attackRadius;
	ifs >> wanderSpeed;
	ifs >> timeToChangeDirection;
	ifs >> pursuitSpeed;
	ifs >> attackDamage;
	ifs >> health;

	AddAnimation(WALKING_ANIM, ifs);
	AddAnimation(ATTACK_ANIM, ifs);
	AddAnimation(DYING_ANIM, ifs);

	SetAnimation(WALKING_ANIM);
}

BadGuy::~BadGuy(void)
{
	delete alertSound;
	delete damageSound;
	delete dyingSound;
	delete attackSound;
}

void BadGuy::DoDamage(int damage)
{
	if(!IsDead())
	{
		health -= damage;
		if(health <= 0)
		{
			health = 0;
			state = DEAD;
			dyingSound->Play();
			this->velocity.x = 0.0f;
			this->velocity.z = 0.0f;
			SetAnimation(DYING_ANIM, false);
		}
		else
		{
			damageSound->Play();
		}
	}
}




void BadGuy::Update(float timeStep)
{
	Character::Update(timeStep);

	switch(state)
	{
		case ROTATING:
		{
			// switch to a random direction
			rotation = (float)(rand()%360);

			Vector3 dir = GetTarget();

			velocity.x = dir.x * wanderSpeed;
			velocity.z = dir.z * wanderSpeed;

			state = WANDERING;
		}
		break;

		case WANDERING:
		{
			timeAccumulator += timeStep;
			if(timeAccumulator > timeToChangeDirection)
			{
				state = ROTATING;

				timeAccumulator = 0.0f;
			}

			// gets the closest character to this one that is not of this type
			this->pursuit = level->GetClosestEnemy(this);

			// if this bad guy sees another character and the other character is not of the same type
			if(pursuit->GetPosition().DistanceTo(this->GetPosition()) < this->alertRadius && !pursuit->IsDead() && !level->AllCoinsCollected())
			{
				// start pursuing
				state = PURSUIT;
				alertSound->Play();
			}
		}
		break;

		case PURSUIT:
		{
			// get the vector from this character to the character
			// in pursuit
			Vector3 dir = pursuit->GetPosition() - this->GetPosition();

			// set the rotation of the badguy and their velocity
			this->rotation = GetRotation(dir);
			Vector3 vel = GetTarget() * pursuitSpeed;
			this->velocity.x = vel.x;
			this->velocity.z = vel.z;

			// distance to player
			float dist = dir.Magnitude();

			// start attacking within a certain distance
			if(dist < attackRadius)
			{
				state = ATTACKING;
				SetAnimation(ATTACK_ANIM, false);
				this->velocity.x = 0.0f;
				this->velocity.z = 0.0f;
			}

			// if they are out of range go back to wandering
			if(dist > alertRadius)
			{
				state = CONFUSED;
				//SetAnimation(CONFUSED_ANIM);
			}

			if(level->AllCoinsCollected())
			{
				state = ROTATING;
			}
		}
		break;

		case CONFUSED:
		{
			state = ROTATING;
			SetAnimation(WALKING_ANIM);
		}
		break;

		case DEAD:
		{
			// no return...
		}
		break;

		case ATTACKING:
		{
			// if the enemy is out of reach go back into pursuit
			float dist = pursuit->GetPosition().DistanceTo(this->GetPosition());
			if(dist > attackRadius)
			{
				state = PURSUIT;
			}

			if(pursuit->IsDead())
			{
				state = ROTATING;
				SetAnimation(WALKING_ANIM);
			}

			if(level->AllCoinsCollected())
			{
				state = ROTATING;
			}

			// the speed of teh animation determines
			// how often the character attacks
			if(currentFrame == endFrame)
			{
				pursuit->DoDamage(attackDamage);
				if(!pursuit->IsDead())
				{
					SetAnimation(ATTACK_ANIM, false);
				}
				attackSound->Play();
			}
		}
		break;
	}



}
