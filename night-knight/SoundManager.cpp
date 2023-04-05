#include "SoundManager.h"

SoundManager::SoundManager() {
	engine = NULL;
}

SoundManager::~SoundManager() {
	engine->drop();
}

void SoundManager::init() {
	engine = createIrrKlangDevice();
	engine->setSoundVolume(0.5f);
}

ISoundEngine* SoundManager::getSoundEngine() {
	return engine;
}

ISound** SoundManager::getBgSoundPtr()
{
	return &bgSound;
}

ISound* SoundManager::changeBgMusic(const char* file, bool loop, bool pause) {
	if (bgSound != nullptr) {
		bgSound->stop();
		bgSound->drop();
	}
	bgSound = engine->play2D(file, loop, pause, true);
	//bgSound->setVolume(0.75);
	return bgSound;
}

void SoundManager::pauseBgMusic(bool pause) {
	if (bgSound != nullptr) {
		bgSound->setIsPaused(pause);
	}
}

void SoundManager::stopBgMusic() {
	if (bgSound != nullptr) {
		bgSound->stop();
		bgSound->drop();
		bgSound = nullptr;
	}
}
