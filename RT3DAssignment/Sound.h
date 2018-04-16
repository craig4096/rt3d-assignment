#pragma once

#include "AL/alut.h"
#include "Vector3.h"
#include <fstream>

class Sound
{
public:

	Sound(std::ifstream& ifs, bool looping);
	~Sound(void);


	void Play();

	void Stop();

private:

	// sound buffer
	ALuint buffer;

	// source
	ALuint source;
};
