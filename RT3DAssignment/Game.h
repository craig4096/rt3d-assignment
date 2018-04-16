#pragma once

#include "Player.h"
#include "Camera.h"
#include "Level.h"


class Game
{
public:
	Game(const char* gamefile);
	~Game(void);

	// updates the game
	void Update(float timeStep);

	// draws the entire game
	void Draw();

	// input handlers
	void KeyPressed(unsigned char c);
	void KeyReleased(unsigned char c);
	void MouseMove(int x, int y);
	void MousePress(int button, int x, int y);
	void MouseReleased(int button, int x, int y);

	// draws a string on the screen
	static void DrawString(const char* str, float x, float y, void* font);

	Player* GetPlayer() { return player; }

	bool IsComplete() const { return completed; }

	static std::string textureDirectory;
	static std::string soundDirectory;
	static std::string modelDirectory;

private:

	// the game character
	Player* player;

	// the camera
	Camera* camera;

	// the current level
	Level* level;

	bool completed;

	// filenames of all levels in the game
	std::vector<std::string> levels;
	unsigned int currentLevel;

};
