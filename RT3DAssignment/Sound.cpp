#include "Sound.h"
#include <exception>
#include <string>
#include "Game.h"
using namespace std;


Sound::Sound(std::ifstream& ifs, bool looping)
{
	string wav;
	ifs >> wav;

	if (!buffer.loadFromFile((Game::soundDirectory + wav).c_str())) {
		throw std::logic_error("could not load sound file");
	}
	sound.setBuffer(buffer);
	sound.setLoop(looping);
}

Sound::~Sound(void)
{
}


void Sound::Play()
{
	sound.play();
}

void Sound::Stop()
{
	sound.stop();
}
