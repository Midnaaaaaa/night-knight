#pragma once

#include <irrKlang.h>
using namespace irrklang;

class SoundManager {

public:
	SoundManager();
	~SoundManager();
	static SoundManager& instance() {
		static SoundManager SM;
		return SM;
	}

	void init();
	ISoundEngine* getSoundEngine();
	ISound** getBgSoundPtr();

	void changeBgMusic(const char* file, bool loop = false, bool pause = false);

private:
	ISoundEngine* engine;
	ISound* bgSound;
};