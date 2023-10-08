#include "Level.h"
#include <fstream>
#include <exception>
#include <GL/freeglut.h>
#include <iostream>
#include <sstream>
#include "Game.h"
#include "BadGuy.h"
using namespace std;



Level::Level(Game* game, const string& filename)
	: coinsCollected(0)
	, lightCount(0)
	, game(game)
	, completed(false)
{
	ifstream ifs(filename.c_str());
	if(!ifs.is_open())
	{
		throw "could not open level file";
	}

	string heightmapBmp, textureBmp;

	ifs >> heightmapBmp;
	ifs >> textureBmp;

	float scale;
	float height;
	float tile;

	ifs >> scale;
	ifs >> height;
	ifs >> tile;

	// load heightmap material
	heightmapMaterial = Material(ifs);

	heightmap = new Heightmap(heightmapBmp.c_str(), textureBmp.c_str(), scale, height, tile);

	ifs >> waitTime;

	// load the skybox
	skybox.Load(ifs);

	coinCollectedSound = new Sound(ifs, false);
	backgroundSound = new Sound(ifs, true);
	levelCompleteSound = new Sound(ifs, false);

	// start playing the background sound
	backgroundSound->Play();

	startPos = Vector3(ifs);

	while(!ifs.eof())
	{
		// load level objects
		std::string objName;
		ifs >> objName;

		if(objName == "BLOCK")
		{
			blocks.push_back(Block(ifs));
		}
		else if(objName == "COIN")
		{
			coins.push_back(Coin(ifs));
		}
		else if(objName == "LIGHT")
		{
			if(lightCount < 8)
			{
				lights[lightCount++] = Light(ifs);
			}
		}
		else if(objName == "BAD_GUY")
		{
			characters.push_back(new BadGuy(this, ifs));
		}
	}

	ifs.close();
}

Level::~Level(void)
{
	// delete all level characters
	for(unsigned int i = 0; i < characters.size(); ++i)
	{
		delete characters[i];
	}

	delete heightmap;
	delete coinCollectedSound;
	delete backgroundSound;
	delete levelCompleteSound;
}

void Level::Update(float timeStep)
{
	if(coinsCollected == coins.size())
	{
		waitTime -= timeStep;
		if(waitTime < 0.0f)
		{
			this->completed = true;
		}
	}


	// update all coins
	for(unsigned int i = 0; i < coins.size(); ++i)
	{
		coins[i].Update(timeStep);
	}

	// update all characters
	for(unsigned int i = 0; i <characters.size(); ++i)
	{
		characters[i]->Update(timeStep);
	}
			
}



Character* Level::GetClosestEnemy(Character* c)
{
	Character* rval = game->GetPlayer();
	float smallestDist = rval->GetPosition().DistanceTo(c->GetPosition());

	for(unsigned int i = 0; i < characters.size(); ++i)
	{
		float dist = characters[i]->GetPosition().DistanceTo(c->GetPosition());
		if(dist < smallestDist && characters[i]->GetType() != c->GetType())
		{
			smallestDist = dist;
			rval = characters[i];
		}
	}
	return rval;
}

void Level::CheckForCollection(const Vector3& pos)
{
	for(unsigned int i = 0; i < coins.size(); ++i)
	{
		if((coins[i].GetPosition() - pos).Magnitude() < 3.0)
		{
			if(!coins[i].IsCollected())
			{
				coinCollectedSound->Play();
				coins[i].Collect();
				coinsCollected++;

				if(coinsCollected == coins.size())
				{
					backgroundSound->Stop();
					levelCompleteSound->Play();
				}
			}
		}
	}
}


void Level::SetLights()
{
	for(unsigned int i = 0; i < 8; ++i)
	{
		if(i < lightCount)
		{
			glEnable(GL_LIGHT0 + i);
			lights[i].Set(GL_LIGHT0 + i);
		}
		else
			glDisable(GL_LIGHT0 + i);
	}
}


void Level::Draw(const Vector3& camPos)
{
	glDisable(GL_LIGHTING);
	// draw skybox
	skybox.Draw(camPos);
	glEnable(GL_LIGHTING);

	heightmapMaterial.Set();
	// draw the heightmap
	heightmap->Draw();

	// draw all the blocks
	for(unsigned int i = 0; i < blocks.size(); ++i)
	{
		blocks[i].Draw();
	}

	// draw all the coins
	for(unsigned int i = 0; i < coins.size(); ++i)
	{
		coins[i].Draw();
	}

	// draw all characters
	for(unsigned int i = 0; i < characters.size(); ++i)
	{
		characters[i]->Draw();
	}

	std::stringstream ss;
	ss << coinsCollected << "/" << coins.size() << " Coins Collected";
	ss << "    ";
	ss << game->GetPlayer()->GetHealth() << " health";

	glDisable(GL_LIGHTING);
	glColor3f(1.0f, 1.0f, 1.0f);
	Game::DrawString(ss.str().c_str(), 10, 10, GLUT_BITMAP_HELVETICA_18);
	glEnable(GL_LIGHTING);
}


Vector3 Level::SlideBox(const Vector3& start, const Vector3& end, const Vector3& size, bool& collidedWithGround) const
{
	float closestTime;
	Plane3d closestPlane;

	Vector3 a = start;
	Vector3 b = end;

	collidedWithGround = false;

	bool collided;
	do
	{
		// first get the bounding box of the path that the box travels through
		BoundingBox3d pathBB(a);
		pathBB.AddPoint(b);

		pathBB.min -= size;
		pathBB.max += size;

		collided = false;
		closestTime = 1.0;


		for(unsigned int i = 0; i < blocks.size(); ++i)
		{
			// check if the blocks bounding box intersects the paths bounding box
			BoundingBox3d blockBB = blocks[i].GetBoundingBox();

			if(pathBB.IntersectsOther( blockBB ))
			{
				// check further for collision
				// extrude the block bounding box so we can check
				// for a collision using a line-box intersection test
				// rather than a box-box test
				blockBB.min -= size;
				blockBB.max += size;

				float time;
				Plane3d plane;
				// now do a line intersection test
				if(blockBB.IntersectsLine(a, b, time, plane))
				{
					// if it intersects - save the closest time
					if(time < closestTime)
					{
						closestTime = time;
						closestPlane = plane;
					}

					collided = true;
				}
			}
		}

		// at this point we have the point of intersection
		// and the equation of the plane that the box is colliding
		// with, we now need to reset the a and b vectors to be:
		// a = poi and b = the original b vector projected onto
		// the plane. (b - a) now represents the sliding vector
		// i.e the direction the box will 'slide' against the wall
		// we repeat this process over and over until the box does
		// not collide with a surface anymore.

		if(collided)
		{
			a = a + ((b - a) * closestTime);
			b = closestPlane.Project(b);

			// extrude vectors slightly from colliding surface
			a += closestPlane.GetNormal() * 0.00001f;
			b += closestPlane.GetNormal() * 0.00001f;


			// check if the surface normal is facing in the general up
			// direction... ifso set collidedWithGround = true else false
			if(closestPlane.GetNormal().DotProduct(Vector3(0.0, 1.0, 0.0)) > 0.5)
			{
				collidedWithGround = true;
			}
			else
			{
				collidedWithGround = false;
			}
		}

	}
	while(collided);

	// get the height of the heightmap
	float height = heightmap->GetHeight(b.x, b.z);

	if(b.y - size.y < height)
	{
		b.y = height + size.y;
		collidedWithGround = true;
	}


	// check for collisions with the boundaries of the map
	float w = this->heightmap->GetXSize() / 2.0f;
	float h = this->heightmap->GetZSize() / 2.0f;

	if(b.x - size.x < -w)
	{
		b.x = -w + size.x + 0.0001f;
	}

	if(b.x + size.x > w)
	{
		b.x = (w - size.x) - 0.0001f;
	}

	if(b.z - size.z < -h)
	{
		b.z = -h + size.z + 0.0001f;
	}

	if(b.z + size.z > h)
	{
		b.z = (h - size.z) + 0.0001f;
	}

	// final position of box will be equal to b
	return b;
}
