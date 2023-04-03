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

private:
	ISoundEngine* engine;
};