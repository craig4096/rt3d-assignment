#include "Game.h"
#include <fstream>
#include <string>
#include <GL/freeglut.h>
#include "TextureLoader.h"
using namespace std;

string Game::textureDirectory;
string Game::soundDirectory;
string Game::modelDirectory;

Game::Game(const char* filename)
	: completed(false)
{
	// open up the file
	std::ifstream ifs(filename);
	if(!ifs.is_open())
	{
		throw "could not open game file!";
	}

	// load in the resource directories
	ifs >> textureDirectory;
	ifs >> soundDirectory;
	ifs >> modelDirectory;


	player = new Player(ifs);

	camera = new Camera(player, ifs);

	// load in all the levels
	while(!ifs.eof())
	{
		std::string level;
		ifs >> level;
		if(!level.empty())
		{
			levels.push_back(level);
		}
	}
	ifs.close();


	// now load the first level
	if(levels.size() < 1)
	{
		throw "game file needs to have at least one level";
	}

	currentLevel = 0;
	level = new Level(this, levels[0].c_str());
	player->SetLevel(level);
}

Game::~Game(void)
{
	// delete all cached models
	MD2Model::ClearCache();
	// delete all cached textures
	TextureLoader::ClearCache();

	delete level;
	delete camera;
	delete player;
}

void Game::Update(float timeStep)
{
	if(level->IsComplete())
	{
		// delete the current level
		delete level;
		level = 0;

		if(currentLevel == levels.size()-1)
		{
			// end of game
			completed = true;
			return;
		}
		else
		{
			level = new Level(this, levels[++currentLevel].c_str());
			player->SetLevel(level);
		}
	}

	// if the player has died we need to reload the current level
	if(player->DeadForAWhile())
	{
		delete level;
		level = new Level(this, levels[currentLevel].c_str());
		player->SetLevel(level);
	}

	camera->Update(timeStep);

	// update the character
	player->Update(timeStep);

	// update the level
	level->Update(timeStep);

	level->CheckForCollection(player->GetPosition());
}


void Game::Draw()
{
	// set the camera
	camera->Set();

	glEnable(GL_LIGHTING);
	level->SetLights();

	level->Draw(camera->GetPosition());
	player->Draw();
	glDisable(GL_LIGHTING);
}




void Game::KeyPressed(unsigned char c)
{
	player->KeyPressed(c);
}

void Game::KeyReleased(unsigned char c)
{
	player->KeyReleased(c);
}


void Game::DrawString(const char* str, float x, float y, void* font)
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, 100, 0, 100);

	glRasterPos2f(x, y);
	while(*str)
	{
		glutBitmapCharacter(font, *str++);
	}

	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}




