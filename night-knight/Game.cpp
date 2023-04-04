#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"

#define TRANSITION_TIME 500


void Game::init()
{
	//Hay que hacer destructora
	SoundManager::instance().init();
	engine = SoundManager::instance().getSoundEngine();
	menuMusic = engine->play2D("sound/menu.mp3", true, false, true);
	menuMusic->setVolume(0.5);
	bgMusicPtr = SoundManager::instance().getBgSoundPtr();

	bPlay = true;
	playing = false;
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	menu.init();
	scene = new Scene(1);
	scene->init();
}

bool Game::update(int deltaTime)
{
	updateTimers(deltaTime);


	if (transitionTimer > 0) return bPlay;
	if (playing && !scene->getPauseState()) scene->update(deltaTime);
	else if(!playing) menu.update(deltaTime);
	if (getKeyUp('m') && playing) {
		scene->changePauseState();
		engine->setAllSoundsPaused(scene->getPauseState());
		menuMusic->setIsPaused(true);
	}


	//Update old keys
	for (int i = 0; i < 256; ++i) {
		old_keys[i] = keys[i];
		old_specialKeys[i] = specialKeys[i];
	}


	return bPlay;
}

void Game::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if (transitionTimer > 0) return;
	if (playing) scene->render();
	else menu.render();
}

void Game::keyPressed(int key)
{
	//if(key == 27) // Escape code
	//	bPlay = false;
	keys[key] = true;
}

void Game::keyReleased(int key)
{
	keys[key] = false;
}

void Game::specialKeyPressed(int key)
{
	specialKeys[key] = true;
}

void Game::specialKeyReleased(int key)
{
	specialKeys[key] = false;
}

void Game::mouseMove(int x, int y)
{
}

void Game::mousePress(int button)
{
}

void Game::mouseRelease(int button)
{
}

bool Game::getKey(int key) const
{
	return keys[key];
}

bool Game::getKeyUp(int key) const {
	return keys[key] && !old_keys[key];
}

bool Game::getSpecialKey(int key) const
{
	return specialKeys[key];
}

bool Game::getSpecialKeyUp(int key) const
{
	return specialKeys[key] && !old_specialKeys[key];
}

void Game::toggleMenu() {
	playing = !playing;
	//(*bgMusicPtr)->setIsPaused(!playing);
	menuMusic->setIsPaused(true);
	transitionTimer = TRANSITION_TIME;
}

void Game::exitLevel()
{
	delete scene;
	scene = new Scene(1);
	scene->init();
	SoundManager::instance().stopBgMusic();
	transitionTimer = TRANSITION_TIME;


	playing = false;
}

void Game::changeLevel(int level)
{
	delete scene;
	scene = new Scene(level);
	scene->init();
	menuMusic->setIsPaused(true);
	transitionTimer = TRANSITION_TIME;
	playing = true;
}

void Game::updateTimers(int deltaTime) {
	if (transitionTimer > 0) {
		transitionTimer -= deltaTime;
		if (transitionTimer <= 0) {
			transitionTimer = 0;
			if (!playing) {
				menuMusic->setPlayPosition(0);
				menuMusic->setIsPaused(false);
			}
		}
	}
}




