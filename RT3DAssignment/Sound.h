#pragma once

#include "Vector3.h"
#include <fstream>
#include <SFML/Audio.hpp>

class Sound
{
public:

	Sound(std::ifstream& ifs, bool looping);
	~Sound(void);


	void Play();

	void Stop();

private:
	sf::SoundBuffer buffer;
	sf::Sound sound;
};
