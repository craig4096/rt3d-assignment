#include "Player.h"
#include <string>
#include "Game.h"
using namespace std;

Player::Player(std::ifstream& ifs)
	: Character(0, ifs)
	, left(false)
	, right(false)
	, forward(false)
	, backward(false)
	, jumping(false)
	, attacking(false)
	, deadTimeAccumulator(0.0f)
{
	ifs >> initialHealth;
	health = initialHealth;
	ifs >> attackDamage;
	jumpSound = new Sound(ifs, false);
	attackSound = new Sound(ifs, false);

	ifs >> speed;

	ifs >> rotateSpeed;

	ifs >> deadTime;


	AddAnimation(WALK, ifs);
	AddAnimation(DIE, ifs);
	AddAnimation(STAND, ifs);
	AddAnimation(JUMP, ifs);
	AddAnimation(ATTACK, ifs);

	SetAnimation(STAND);
}

Player::~Player(void)
{
	delete attackSound;
	delete jumpSound;
}


void Player::SetLevel(Level* level)
{
	this->level = level;
	position = level->GetStartPos();
	velocity = Vector3(0.0f, 0.0f, 0.0f);
	SetAnimation(STAND);
	health = initialHealth;
	deadTimeAccumulator = 0.0f;
}


void Player::DoDamage(int damage)
{
	health -= damage;
	if(health <= 0)
	{
		health = 0;
		// game over
		velocity.x = 0.0f;
		velocity.z = 0.0f;
		SetAnimation(DIE, false);
	}
}


void Player::Attack()
{
	attackSound->Play();
	for(unsigned int i = 0; i < level->GetCharacterCount(); ++i)
	{
		if(level->GetCharacter(i)->GetPosition().DistanceTo(GetPosition()) < 10.0)
		{
			level->GetCharacter(i)->DoDamage(attackDamage);
		}
	}
}

void Player::Update(float timeStep)
{
	if(IsDead())
	{
		deadTimeAccumulator += timeStep;
	}
	else
	{
		// turning left & right
		if(left)
		{
			this->rotation -= rotateSpeed * timeStep;
		}

		if(right)
		{
			this->rotation += rotateSpeed * timeStep;
		}
		
		if(!jumping)
		{
			if(attacking)
			{
				if(currentAnimation != ATTACK)
				{
					SetAnimation(ATTACK, false);
					Attack();
				}

				if(currentFrame == endFrame)
				{
					attacking = false;
					SetAnimation(WALK);
				}
			}

			if(currentAnimation == JUMP)
			{
				SetAnimation(STAND);
			}

			// only move forward and backward if the character is on the ground
			if(collidedWithGround)
			{
				if(forward && !attacking)
				{
					velocity = GetTarget() * speed;

					if(currentAnimation != WALK)
					{
						SetAnimation(WALK);
					}
				}
				else
				{
					velocity = Vector3(0,0,0);
					if(currentAnimation == WALK)
					{
						SetAnimation(STAND);
					}
				}

				if(backward)
				{
					velocity = GetTarget() * -speed;
				}
			}
		}
		else
		{
			if(currentAnimation != JUMP)
			{
				jumpSound->Play();
				SetAnimation(JUMP, false);

				attacking = false;

				this->acceleration = Vector3(0, 400, 0);
			}
		}
	}

	Character::Update(timeStep);

	if(collidedWithGround)
	{
		// if collided with ground
		jumping = false;
	}
}


void Player::KeyPressed(unsigned char c)
{
	switch(c)
	{
		case 'w': forward = true;	break;
		case 's': backward = true;	break;
		case 'a': left = true;		break;
		case 'd': right = true;		break;
		case ' ': jumping = true;	break;
		case 'e': attacking = true; break;
	}
}

void Player::KeyReleased(unsigned char c)
{
	switch(c)
	{
		case 'w': forward = false;	break;
		case 's': backward = false;	break;
		case 'a': left = false;		break;
		case 'd': right = false;	break;
	}
}