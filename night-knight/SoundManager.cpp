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