#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"

#define TRANSITION_TIME 500


void Game::init()
{
	initShaders();

	float rectangleVertices[] =
	{
		// Coords    // texCoords
		 1.0f, -1.0f,  1.0f, 0.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		-1.0f,  1.0f,  0.0f, 1.0f,

		 1.0f,  1.0f,  1.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		-1.0f,  1.0f,  0.0f, 1.0f
	};

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rectangleVertices), &rectangleVertices, GL_STATIC_DRAW);
	posLocation = postProcessingProgram.bindVertexAttribute("position", 2, 4 * sizeof(float), 0);
	texCoordLocation = postProcessingProgram.bindVertexAttribute("texCoord", 2, 4 * sizeof(float), (void*)(2 * sizeof(float)));




	//Hay que hacer destructora
	SoundManager::instance().init();
	engine = SoundManager::instance().getSoundEngine();
	menuMusic = engine->play2D("sound/menu.mp3", true, false, true);
	menuMusic->setVolume(0.5);
	bgMusicPtr = SoundManager::instance().getBgSoundPtr();

	glGenFramebuffers(1, &FBO);
	frameBufferTexture.loadFromFrameBuffer(FBO);

	bPlay = true;
	playing = false;
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	menu.init();
	scene = new Scene(1);
	scene->init();

	currentTime = 0;
}

bool Game::update(int deltaTime)
{
	updateTimers(deltaTime);
	currentTime += deltaTime;

	if (transitionTimer > 0) return bPlay;
	if (playing && !scene->getPauseState()) scene->update(deltaTime);
	else if(!playing) menu.update(deltaTime);
	if (getKeyUp('m') && playing) {
		scene->changePauseState();
		engine->setAllSoundsPaused(scene->getPauseState());
	}
	if (getKeyUp('1')) changeLevel(1);
	else if (getKeyUp('2')) changeLevel(2);
	else if (getKeyUp('3')) changeLevel(3);
	else if (getKeyUp('4')) changeLevel(4);
	else if (getKeyUp('5')) changeLevel(5);
	else if (getKeyUp('6')) changeLevel(6);


	//Update old keys
	for (int i = 0; i < 256; ++i) {
		old_keys[i] = keys[i];
		old_specialKeys[i] = specialKeys[i];
	}


	return bPlay;
}

void Game::render()
{
	
	//int vp[4];
	//glGetIntegerv(GL_VIEWPORT, vp);
	//frameBufferTexture.use();
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, vp[2], vp[3], 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

	// Bind the custom framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if (transitionTimer > 0) return;
	if (playing) scene->render();
	else menu.render();

	// Bind the default framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// Draw the framebuffer rectangle
	postProcessingProgram.use();
	postProcessingProgram.setUniform1i("effectTimer", currentTime);

	glDisable(GL_DEPTH_TEST); // prevents framebuffer rectangle from being discarded

	glEnable(GL_TEXTURE_2D);
	frameBufferTexture.use();
	glBindVertexArray(vao);
	glEnableVertexAttribArray(posLocation);
	glEnableVertexAttribArray(texCoordLocation);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisable(GL_TEXTURE_2D);

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
	if (menuMusic != nullptr) {
		menuMusic->stop();
		menuMusic->drop();
		menuMusic = nullptr;
	}
	transitionTimer = TRANSITION_TIME;
}

void Game::exitLevel()
{
	delete scene;
	puntuacionActual = 0;
	vidasActuales = 3;
	scene = new Scene(1);
	scene->init();
	SoundManager::instance().stopBgMusic();
	engine->stopAllSounds();
	transitionTimer = TRANSITION_TIME;

	playing = false;
}

void Game::changeLevel(int level)
{
	delete scene;
	scene = new Scene(level);
	scene->init();
	if (menuMusic != nullptr) {
		menuMusic->stop();
		menuMusic->drop();
		menuMusic = nullptr;
	}
	engine->stopAllSounds();
	transitionTimer = TRANSITION_TIME;
	playing = true;
}

void Game::savePuntuacionYVidas(int puntuacion, int vidas)
{
	this->puntuacionActual = puntuacion;
	this->vidasActuales = vidas;
}

int Game::getVidasActuales()
{
	return vidasActuales;
}

int Game::getPuntuacionActual()
{
	return puntuacionActual;
}

void Game::resize(int w, int h)
{
	frameBufferTexture.use();
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

	glViewport(0, 0, w, h);
}

void Game::updateTimers(int deltaTime) {
	if (transitionTimer > 0) {
		transitionTimer -= deltaTime;
		if (transitionTimer <= 0) {
			transitionTimer = 0;
			if (!playing) {
				menuMusic = engine->play2D("sound/menu.mp3", true, false, true);
				menuMusic->setVolume(0.5);
			}
		}
	}
}


bool Game::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/postproc.vert");
	if (!vShader.isCompiled())
	{
		return false;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/postproc.frag");
	if (!fShader.isCompiled())
	{
		return false;
	}
	postProcessingProgram.init();
	postProcessingProgram.addShader(vShader);
	postProcessingProgram.addShader(fShader);
	postProcessingProgram.link();
	if (!postProcessingProgram.isLinked())
	{
		return false;
	}
	postProcessingProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();

	return true;
}

