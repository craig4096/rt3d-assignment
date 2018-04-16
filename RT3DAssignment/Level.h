#pragma once
#include <vector>
#include "Coin.h"
#include "Heightmap.h"
#include "Block.h"
#include "SkyBox.h"
#include "BoundingBox3d.h"
#include "Sound.h"
#include "Light.h"
#include <string>
#include "Character.h"
class Game;

class Level
{
public:
	Level(Game* game, const std::string& filename);
	~Level(void);


	// slides a bounding box through the current level, returns the nmew position of the box
	// didCollide returns whether the box collided with anything
	Vector3 SlideBox(const Vector3& start, const Vector3& end, const Vector3& size, bool& collidedWithGround) const;


	void Update(float timeStep);

	void Draw(const Vector3& camPos);

	// checks the the characters position to see if they are close enough to
	// a coin for it to be collected, if it is then the coin will be collected
	void CheckForCollection(const Vector3& pos);

	// returns the closest character to character c that is not of the same type
	Character* GetClosestEnemy(Character* c);

	// sets the lights of this level
	void SetLights();

	const Vector3 GetStartPos() const { return startPos; }

	bool IsComplete() const { return completed; }

	unsigned int GetCharacterCount() const { return characters.size(); }
	Character* GetCharacter(unsigned int i) { return characters[i]; }

	Heightmap* GetHeightmap() const { return heightmap; }

	bool AllCoinsCollected() const { return coinsCollected == coins.size(); }

private:

	Game* game;

	// after the level is complete it will wait for
	// 'waitTime' amount of seconds before calling Game::NextLevel
	float waitTime;

	// the numbers of coins that have been collected
	// so far
	unsigned int coinsCollected;

	// the players starting position in the level
	Vector3 startPos;

	Sound* backgroundSound;
	Sound* coinCollectedSound;
	Sound* levelCompleteSound;

	// the heightmap
	Heightmap* heightmap;

	// the heightmaps material
	Material heightmapMaterial;

	SkyBox skybox;

	// the level characters
	std::vector<Character*> characters;

	std::vector<Coin> coins;
	std::vector<Block> blocks;


	Light lights[8];
	unsigned int lightCount;

	bool completed;
};
