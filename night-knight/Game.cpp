#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"

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
	if (playing) scene->update(deltaTime);
	else menu.update(deltaTime);
	if (getKeyUp('m')) {
		toggleMenu();
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
	(*bgMusicPtr)->setIsPaused(!playing);
	menuMusic->setIsPaused(true);
}

void Game::exitLevel()
{
	delete scene;
	scene = new Scene(1);
	scene->init();
	(*bgMusicPtr)->setPlayPosition(0);
	(*bgMusicPtr)->setIsPaused(true);
	menuMusic->setPlayPosition(0);
	menuMusic->setIsPaused(false);

	playing = false;
}

void Game::changeLevel(int level)
{
	scene = new Scene(level);
	scene->init();
	menuMusic->setIsPaused(true);
}



