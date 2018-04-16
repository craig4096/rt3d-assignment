#include "Sound.h"
#include <exception>
#include <string>
#include "Game.h"
using namespace std;


Sound::Sound(std::ifstream& ifs, bool looping)
{
	string wav;
	ifs >> wav;

	buffer = alutCreateBufferFromFile((Game::soundDirectory + wav).c_str());
	if(buffer == AL_NONE)
	{
		throw "could not load sound file";
	}

	// create the source and bind it to the buffer
	alGenSources(1, &source);
	alSourcei(source, AL_BUFFER, buffer);

	alSourcei(source, AL_LOOPING, looping ? 1 : 0);
}

Sound::~Sound(void)
{
	alDeleteSources(1, &source);
	alDeleteBuffers(1, &buffer);
}


void Sound::Play()
{
	alSourcePlay(source);
}

void Sound::Stop()
{
	alSourceStop(source);
}
