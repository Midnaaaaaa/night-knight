#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"

#define TRANSITION_TIME 500

enum PostEffects {
	PEFFECT_FADE
};



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
	currentLevel = 0;
	targetLevel = 0;
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	menu.init();

	currentTime = 0;
	postEffect.id = -1;
}

bool Game::update(int deltaTime)
{
	updateTimers(deltaTime);
	currentTime += deltaTime;

	if (transitionTimer > 0) return bPlay;
	if (playing() && !scene->getPauseState()) scene->update(deltaTime);
	else if(!playing()) menu.update(deltaTime);
	if (getKeyUp('p') && playing()) {
		scene->changePauseState();
		engine->setAllSoundsPaused(scene->getPauseState());
	}
	else if (getKeyUp(27) && playing()) {
		exitLevel();
	}
	if (getKeyUp('1')) changeLevel(1);
	else if (getKeyUp('2')) changeLevel(2);
	else if (getKeyUp('3')) changeLevel(3);
	else if (getKeyUp('4')) changeLevel(4);
	else if (getKeyUp('5')) changeLevel(5);
	else if (getKeyUp('6')) changeLevel(6);
	else if (getKeyUp('7')) changeLevel(7);
	else if (getKeyUp('8')) changeLevel(8);


	//Update old keys
	for (int i = 0; i < 256; ++i) {
		old_keys[i] = keys[i];
		old_specialKeys[i] = specialKeys[i];
	}


	return bPlay;
}

void Game::render()
{	
	// Bind the custom framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if (playing()) scene->render();
	else menu.render();

	// Bind the default framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// Draw the framebuffer rectangle
	postProcessingProgram.use();
	//postProcessingProgram.setUniform1i("effectTimer", currentTime);
	int effectTimer = postEffect.timer;
	if (postEffect.timer < 0) {
		effectTimer = postEffect.timer + postEffect.duration; // [-duration 0] ---> [0 duration] 
	}
	postProcessingProgram.setUniform1i("effectId", postEffect.id);
	postProcessingProgram.setUniform1i("effectTimer", effectTimer);
	postProcessingProgram.setUniform1i("effectDuration", postEffect.duration);
	postProcessingProgram.setUniform4f("color", 1.f, 1.f, 1.f, 1.f);

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

void Game::exitLevel()
{
	targetLevel = 0;
	transitionTimer = TRANSITION_TIME;
	addPostEffect(PEFFECT_FADE, TRANSITION_TIME);
}

void Game::changeLevel(int level)
{
	targetLevel = level;
	transitionTimer = TRANSITION_TIME;
	addPostEffect(PEFFECT_FADE, TRANSITION_TIME);
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

void Game::theEnd()
{
	menu.theEnd();
	SoundManager::instance().changeBgMusic("sound/theEnd.mp3", true, false);
	puntuacionActual = 0;
	vidasActuales = 3;
	addPostEffect(PEFFECT_FADE, -5000);
}

void Game::updateTimers(int deltaTime) {
	if (transitionTimer > 0) {
		transitionTimer -= deltaTime;
		if (transitionTimer <= 0) {
			transitionTimer = 0;
			currentLevel = targetLevel;

			if (currentLevel == 0) {
				if (scene != nullptr) 
					delete scene;
				scene = nullptr;
				puntuacionActual = 0;
				vidasActuales = 3;
				SoundManager::instance().stopBgMusic();
				engine->stopAllSounds();



				menuMusic = engine->play2D("sound/menu.mp3", true, false, true);
				menuMusic->setVolume(0.5);
			}
			else if (currentLevel == 9) {
				if (scene != nullptr)
					delete scene;
				scene = nullptr;
				SoundManager::instance().stopBgMusic();
				engine->stopAllSounds();
				theEnd();
			}
			else {
				if (scene != nullptr)
					delete scene;
				scene = new Scene(currentLevel);
				scene->init();
				if (menuMusic != nullptr) {
					menuMusic->stop();
					menuMusic->drop();
					menuMusic = nullptr;
				}
				engine->stopAllSounds();
			}
		}
	}

	if (postEffect.timer > 0) {
		postEffect.timer -= deltaTime;
		if (postEffect.timer <= 0) {
			postEffect.timer = 0;
			postEffect.id = -1;
		}
	}
	else if (postEffect.timer < 0) {
		postEffect.timer += deltaTime;
		if (postEffect.timer >= 0) {
			postEffect.timer = 0;
			postEffect.id = -1;
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

void Game::addPostEffect(int id, int duration)
{
	postEffect.id = id;
	postEffect.timer = duration;
	postEffect.duration = duration;
	if (duration < 0)
		postEffect.duration = -duration;
}

void Game::addPostEffect(int id, int duration, const glm::ivec2& point)
{
	postEffect.id = id;
	postEffect.point = point;
	postEffect.timer = duration;
	postEffect.duration = duration;
}

bool Game::playing()
{
	return currentLevel > 0 && currentLevel != 9;
}

