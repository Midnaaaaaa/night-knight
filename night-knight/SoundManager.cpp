#include "SoundManager.h"

SoundManager::SoundManager() {
	engine = NULL;
}

SoundManager::~SoundManager() {
	engine->drop();
}

void SoundManager::init() {
	engine = createIrrKlangDevice();
}

ISoundEngine* SoundManager::getSoundEngine() {
	return engine;
}

ISound** SoundManager::getBgSoundPtr()
{
	return &bgSound;
}

void SoundManager::changeBgMusic(const char* file, bool loop, bool pause) {
	if (bgSound != nullptr) {
		bgSound->stop();
		bgSound->drop();
	}
	bgSound = engine->play2D(file, loop, pause, true);
	bgSound->setVolume(0.5);
}