#pragma once
#include <fstream>
#include "Vector3.h"

class SkyBox
{
public:
	SkyBox(void);
	~SkyBox(void);


	// loads the skybox from a file
	void Load(std::ifstream& ifs);

	void Draw(const Vector3& camPos);

private:

	unsigned int textures[6];

	// the normalized size of half a pixel
	// assuming the skybox textures are square (width == height)
	float hp;
};
