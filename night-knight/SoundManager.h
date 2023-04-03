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

	ISound* changeBgMusic(const char* file, bool loop = false, bool pause = false);
	void pauseBgMusic(bool pause);
	void stopBgMusic();

private:
	ISoundEngine* engine;
	ISound* bgSound;
};