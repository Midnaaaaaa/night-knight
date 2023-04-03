#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"
#include "Esquelet.h"
#include "Vampir.h"
#include "Fantasma.h"
#include <iomanip>
#include <sstream>

#define MIN_TIME_WITHOUT_SPAWN 1  * 1000
#define MAX_TIME_WITHOUT_SPAWN 1 * 1000

#define MIN_TIME_TO_DESPAWN 8  * 1000
#define MAX_TIME_TO_DESPAWN 16 * 1000

#define SCREEN_X 32*2
#define SCREEN_Y 16*2

#define STAGE_TIMER 60000




enum KeyAnimations {
	IDLE_KEY
};

enum DoorAnimations {
	DOOR_CLOSED, DOOR_OPENED
};

enum DoorParticleAnimations {
	DOOR_PARTICLE
};

enum ClockAnimations {
	IDLE_CLOCK
};

enum Items {
	HOURGLASS, GEM, CLOCK
};

enum CorAnimations{
	COR_FULL, COR_75, COR_50, COR_POCHO	
};



Scene::Scene(int level)
{
	map = NULL;
	player = NULL;
	this->level = level;
}

Scene::~Scene()
{
	if(map != NULL)
		delete map;
	if(player != NULL)
		delete player;
	if (cor != NULL)
		delete cor;
	if (key != nullptr) {
		key->free();
	}
	if (door != nullptr) {
		door->free();
	}
	for (Enemy* e : enemies) {
		delete e;
	}
	while (!objects.empty()) {
		Item& item = objects.front();
		objects.pop_front();
		item.sprite->free();
	}
	texProgram.free();

	if (puntIncr != nullptr) puntIncr->drop();

	if (hourglassSound != nullptr) hourglassSound->drop();

	engine->removeSoundSource(gemSoundSrc);
	engine->removeSoundSource(clockSoundSrc);
}


void Scene::init()
{
	stageTimer = STAGE_TIMER;
	stageCompleted = false;
	stageCompletedTimer = 4000;
	gameOverTimer = 5000;
	freezeTimer = 0;

	initShaders();

	map = TileMap::createTileMap("levels/level" + to_string(level) + ".txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	objectsSpritesheet.loadFromFile("images/items.png", TEXTURE_PIXEL_FORMAT_RGBA);
	bgSpritesheet.loadFromFile("images/bg" + to_string(level) + ".png", TEXTURE_PIXEL_FORMAT_RGBA);
	doorSpritesheet.loadFromFile("images/door.png", TEXTURE_PIXEL_FORMAT_RGBA);
	particleSpritesheet.loadFromFile("images/particles.png", TEXTURE_PIXEL_FORMAT_RGBA);
	corSpritesheet.loadFromFile("images/cor.png", TEXTURE_PIXEL_FORMAT_RGBA);

	bg = Sprite::createSprite(glm::ivec2(SCREEN_X, SCREEN_Y), glm::ivec2(32*map->getTileSize(), 22*map->getTileSize()), glm::ivec2(1,1), &bgSpritesheet, &texProgram);
	//bg->setPosition(glm::ivec2(SCREEN_X, SCREEN_Y));
	
	initPlayerPos = glm::vec2(2, 9);
	doorPos = glm::vec2(20, 3);
	keyPos = glm::vec2(28.5, 18.5);

	//Enemigos
	Vampir *prueba = new Vampir();
	prueba->init(glm::ivec2(SCREEN_X, SCREEN_Y), false, glm::vec2(10, 1), map, texProgram);
	enemies.push_back(prueba);

	
	Fantasma* prueba1 = new Fantasma();
	prueba1->init(glm::ivec2(SCREEN_X, SCREEN_Y), true, glm::vec2(10, 6), map, texProgram);
	enemies.push_back(prueba1);
	

	Esquelet* prueba2 = new Esquelet();
	prueba2->init(glm::ivec2(SCREEN_X, SCREEN_Y), true, glm::vec2(10,6), map, texProgram);
	enemies.push_back(prueba2);


	//Player
	player = new Player();
	player->setTileMap(map);
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), true, "images/soma-animations.png", glm::ivec2(16,32), glm::ivec2(8,32), glm::ivec2(32,64), glm::vec2(1/16.f, 1/16.f), texProgram);

	//player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
	//player->setSpeed(2);


	//Objetos (sprites)
	key = nullptr;
	spawnDoor();
	spawnCor();

	projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);
	currentTime = 0.0f;

	keyCollected = false;

	// Select which font you want to use
	if (!text.init("fonts/ArcadeClassic.ttf", glm::ivec2(SCREEN_X, SCREEN_Y), projection))
		//if(!text.init("fonts/OpenSans-Bold.ttf"))
		//if(!text.init("fonts/DroidSerif.ttf"))
		cout << "Could not load font!!!" << endl;

	string levelSoundFile = "sound/lvl" + to_string(level) + ".mp3";
	engine = SoundManager::instance().getSoundEngine();
	bgSound = SoundManager::instance().changeBgMusic(levelSoundFile.c_str(), true, true);

	puntIncr = engine->play2D("sound/bit.wav", true, true);
	puntIncr->setVolume(0.4);

	hourglassSound = engine->play2D("sound/hourglass.mp3", false, true);
	gemSoundSrc = engine->addSoundSourceFromFile("sound/gem.wav");
	clockSoundSrc = engine->addSoundSourceFromFile("sound/clock.mp3");

}

void Scene::updateTimers(int deltaTime) {
	if (freezeTimer != 0) {
		freezeTimer -= deltaTime;
		if (freezeTimer <= 0) {
			freezeTimer = 0;
			hourglassSound->setIsPaused(true);
			bgSound->setVolume(1);
		}
	}
}

void Scene::update(int deltaTime)
{
	updateTimers(deltaTime);
	currentTime += deltaTime;

	if (stageCompleted) {
		stageCompletedTimer -= deltaTime;
		if (particleDoor != nullptr) particleDoor->update(deltaTime);
		if (stageCompletedTimer <= 3500 && enemies.size() > 0) {
			for (int i = enemies.size() - 1; i >= 0; --i)
			{
				delete enemies[i];
				enemies.pop_back();
			}
		}
		if (stageTimer / 1000 > 0 && stageCompletedTimer <= 1000) {
			int stageTimerActual = stageTimer / 1000;
			stageTimer -= deltaTime * 20;
			player->increasePuntuacion((stageTimerActual - stageTimer/1000)*10);
			if (puntIncr->getIsPaused()) puntIncr->setIsPaused(false);
		}
		else if (stageTimer / 1000 <= 0) {
			puntIncr->setIsPaused(true);
			Game::instance().exitLevel();
		}
		return;
	}
	else if (gameOver) {
		gameOverTimer -= deltaTime;
		player->update(deltaTime);
		if (gameOverTimer <= 0) {
			Game::instance().exitLevel();
		}
		return;
	}
	
	stageTimer -= deltaTime;
	if (spawnTimer == -1) {
		spawnTimer = rand() % (MAX_TIME_WITHOUT_SPAWN - MIN_TIME_WITHOUT_SPAWN + 1) + MIN_TIME_WITHOUT_SPAWN;
		spawnTimer += currentTime;
	}
	if (currentTime >= spawnTimer) {
		int objectToSpawn = rand() % 3;
		glm::ivec2 platform = map->getRandomPlatform();
		switch (objectToSpawn)
		{
		case HOURGLASS:
			Item hourglass = spawnHourglass(glm::vec2(platform.x + 0.5, platform.y - 1.5));
			objects.push_back(hourglass);
			break;
		case GEM:
			Item gem = spawnGem(glm::vec2(platform.x + 0.5, platform.y - 1.5));
			objects.push_back(gem);
			break;
		case CLOCK:
			Item clock = spawnClock(glm::vec2(platform.x + 0.5, platform.y - 1.5));
			objects.push_back(clock);
			break;
		}
		despawnTimer = rand() % (MAX_TIME_TO_DESPAWN - MIN_TIME_TO_DESPAWN + 1) + MIN_TIME_TO_DESPAWN;
		despawnTimer += currentTime;
		spawnTimer = -1;
	}
	if (currentTime >= despawnTimer && despawnTimer != -1) {
		if (!objects.empty()) {
			Item obj;
			obj = objects.front();
			objects.pop_front();
			obj.sprite->free();
		}
		despawnTimer = -1;
	}


	if (Game::instance().getKeyUp('k') && !keyCollected) {
		spawnKey();
	}
	if (Game::instance().getKeyUp('g')) {
		player->setGodMode(!player->inGodMode());
	}

	for (Enemy* e : enemies)
	{
		e->update(deltaTime);
		glm::ivec2 topLeft = e->getColliderPos();
		glm::ivec2 bottomRight = topLeft + e->getColliderSize();
		if (!player->isHurted()) {
			bool wasHit = player->checkCollisionWithRect(topLeft, bottomRight, 1);
			if (wasHit) asesino = e;
		}
	}

	if (map->getNumOfTilesRemaining() == 0 && !keyCollected && key == nullptr) {
		spawnKey();
	}

	player->update(deltaTime);

	//Update llave
	if (key != nullptr && !keyCollected) {
		key->update(deltaTime);
		glm::vec2 topLeft = key->getPosition();
		glm::vec2 bottomRight = topLeft + key->getSpriteSize();
		if (player->checkCollisionWithRect(topLeft, bottomRight, 2)) {
			keyCollected = true;
			key->free();
			key = nullptr;
			door->changeAnimation(DOOR_OPENED, 0);
		}
	}

	//Collision with items
	for (std::deque<Item>::iterator it = objects.begin(); it != objects.end(); ++it) {
		it->sprite->update(deltaTime);
		glm::vec2 topLeft = it->sprite->getPosition();
		glm::vec2 bottomRight = topLeft + it->sprite->getSpriteSize();
		if (player->checkCollisionWithRect(topLeft, bottomRight, 2)) {
			ik_f32 vol;
			switch (it->id)
			{
			case GEM:
				player->increasePuntuacion(500);
				engine->play2D(gemSoundSrc);
				break;
			case HOURGLASS:
				for (Enemy* e : enemies)
				{
					e->freeze(5000, true);
				}
				freezeTimer = 5000;
				//restart sound
				if (hourglassSound == nullptr || hourglassSound->isFinished()) {
					hourglassSound = engine->play2D("sound/hourglass.mp3");
				}
				else {
					hourglassSound->setPlayPosition(0);
					hourglassSound->setIsPaused(false);
				}
				vol = bgSound->getVolume();
				bgSound->setVolume(0.4);
				break;
			case CLOCK:
				stageTimer += 15000;
				engine->play2D(clockSoundSrc);
				break;
			}
			it->sprite->free();
			it = objects.erase(it);
			if (it == objects.end()) break;
		}
	}

	door->update(deltaTime);
	//Colision puerta
	if (door->animation() == DOOR_OPENED) {
		glm::vec2 topLeft = door->getPosition();
		glm::vec2 bottomRight = topLeft + door->getSpriteSize();
		if (player->checkCollisionWithRect(topLeft, bottomRight, 2)) {
			stageCompleted = true;
			glm::vec2 pos = player->getPosition();
			spawnDoorParticle(pos);
		}
	}
	if (player->isGameOver() || stageTimer <= 0 && player->getVidas() == 1) gameOver = true;
	else if (stageTimer <= 0) {
		player->muelto();
		stageTimer = STAGE_TIMER;
	}


	//UPDATE TIMERS
	//if (timer >= currentTime) {
	//	(this->*timerFunc)();
	//}

	//if (player->isGameOver()) {
	//	timer = 4000;
	//	timerFunc = gameOver;
	//}
	
}

void Scene::render()
{
	//Render de game over

	if (gameOver) {
		player->render();
		
		if(stageTimer <= 0) text.render(to_string(stageTimer / 1000), glm::vec2(SCREEN_WIDTH / 2, 30.f), 32, glm::vec4(0.7, 0.2, 0.1, 1), Text::CENTERED);
		else if (asesino != nullptr) asesino->render();
		if (gameOverTimer <= 3000) {
			text.render("GAME    OVER", glm::vec2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2), 32, glm::vec4(1, 1, 1, 1), Text::CENTERED);
		}
		return;
	}

	//La misma camara ortogonal para TODOS
	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);


	//Render normal
	bg->render();

	glm::mat4 modelview;

	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	texProgram.setUniform1i("effectId", -1);
	texProgram.setUniform1i("effectTimer", 0);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	map->render();
	door->render();
	player->render();
	cor->render();
	if (particleDoor != nullptr) {
		particleDoor->render();
	}

	for (Enemy* e : enemies)
	{
		e->render();
	}

	if (key != nullptr) {
		key->render();
	}

	for (Item i : objects)
	{
		i.sprite->render();
	}

	//Render de num vidas
	text.render("x" + to_string(player->getVidas()), glm::vec2(SCREEN_X + 26, 30.f), 26, glm::vec4(1, 1, 1, 1));
	
	//Render de puntuacion
	stringstream ss;

	ss << setw(6) << setfill('0') << player->getPuntuacion();
	text.render(ss.str(), glm::vec2(150.f, 30.f), 26, glm::vec4(1, 1, 1, 1));


	if (stageTimer < 20000 && stageTimer > 5000) {
		text.render(to_string(stageTimer / 1000), glm::vec2(SCREEN_WIDTH / 2, 30.f), 32, glm::vec4(1, 1, 0, 1), Text::CENTERED);
	}
	else if (stageTimer < 5000){
		text.render(to_string(stageTimer / 1000), glm::vec2(SCREEN_WIDTH / 2, 30.f), 32, glm::vec4(0.7, 0.2, 0.1, 1), Text::CENTERED);
	}
	else {
		text.render(to_string(stageTimer / 1000), glm::vec2(SCREEN_WIDTH / 2, 30.f), 32, glm::vec4(1, 1, 1, 1), Text::CENTERED);
	}


	if (stageCompletedTimer <= 3000 && stageCompleted) {
		text.render("STAGE    CLEAR", glm::vec2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2), 26, glm::vec4(1, 1, 1, 1), Text::CENTERED);
	}

	text.render("STAGE " + to_string(level), glm::vec2(SCREEN_X + map->getTileSize() * 32, 30.f), 26, glm::vec4(1, 1, 1, 1), Text::RIGHT_ALIGNED);
}

void Scene::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if(!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if(!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	texProgram.init();
	texProgram.addShader(vShader);
	texProgram.addShader(fShader);
	texProgram.link();
	if(!texProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << texProgram.log() << endl << endl;
	}
	texProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
}

void Scene::spawnKey() {
	key = Sprite::createSprite(glm::ivec2(SCREEN_X, SCREEN_Y), glm::vec2(16, 16), glm::vec2(1/8.f, 1/8.f), &objectsSpritesheet, &texProgram);
	key->setDisplacement(glm::vec2(1 / 8.f * 2, 1 / 8.f * 1));
	key->setPosition(glm::ivec2(keyPos.x * map->getTileSize(), keyPos.y * map->getTileSize()));
	key->addEffect(EFFECT_SIN_Y, 120 * 1000);
}


void Scene::spawnDoor() {
	door = Sprite::createSprite(glm::ivec2(SCREEN_X, SCREEN_Y), glm::vec2(32, 32), glm::vec2(1 / 4.f, 1 / 4.f), &doorSpritesheet, &texProgram);
	door->setDisplacement(glm::vec2(0.0f, 0.0f));
	door->setPosition(glm::ivec2(doorPos.x * map->getTileSize(), doorPos.y * map->getTileSize()));
	door->setNumberAnimations(2);
	door->setAnimationParams(DOOR_CLOSED, 1, false);
	door->addKeyframe(DOOR_CLOSED, glm::vec2(0.0f, 0.0f));

	door->setAnimationParams(DOOR_OPENED, 6, false, 2);
	door->addKeyframe(DOOR_OPENED, glm::vec2(1 / 4.f * 1, 0.0f));
	door->addKeyframe(DOOR_OPENED, glm::vec2(1 / 4.f * 2, 0.0f));
	door->addKeyframe(DOOR_OPENED, glm::vec2(1 / 4.f * 3, 0.0f));

	door->changeAnimation(DOOR_CLOSED, 0);
}



Item Scene::spawnHourglass(glm::vec2 pos) {
	Item hourglass;
	hourglass.sprite = Sprite::createSprite(glm::ivec2(SCREEN_X, SCREEN_Y), glm::vec2(16, 16), glm::vec2(1 / 8.f, 1 / 8.f), &objectsSpritesheet, &texProgram);
	hourglass.id = HOURGLASS;
	hourglass.sprite->setDisplacement(glm::vec2(1/8.f * 5, 0.0f));
	hourglass.sprite->setPosition(glm::ivec2 (pos.x * map->getTileSize(), pos.y * map->getTileSize()));
	hourglass.sprite->addEffect(EFFECT_SIN_Y, 120 * 1000);

	return hourglass;
}

Item Scene::spawnGem(glm::vec2 pos) {
	Item gem;
	gem.sprite = Sprite::createSprite(glm::ivec2(SCREEN_X, SCREEN_Y), glm::vec2(16, 16), glm::vec2(1 / 8.f, 1 / 8.f), &objectsSpritesheet, &texProgram);
	gem.id = GEM;
	gem.sprite->setDisplacement(glm::vec2(1 / 8.f * 3, 1 / 8.f * 2));
	gem.sprite->setPosition(glm::ivec2(pos.x * map->getTileSize(), pos.y * map->getTileSize()));
	gem.sprite->addEffect(EFFECT_SIN_Y, 120 * 1000);

	return gem;
}

Item Scene::spawnClock(glm::vec2 pos) {
	Item clock;
	clock.sprite = Sprite::createSprite(glm::ivec2(SCREEN_X, SCREEN_Y), glm::vec2(16, 16), glm::vec2(1 / 8.f, 1 / 8.f), &objectsSpritesheet, &texProgram);
	clock.id = CLOCK;
	clock.sprite->setDisplacement(glm::vec2(1 / 8.f * 0, 1 / 8.f * 3));
	clock.sprite->setPosition(glm::ivec2(pos.x * map->getTileSize(), pos.y * map->getTileSize()));
	clock.sprite->setNumberAnimations(1);
	clock.sprite->setAnimationParams(IDLE_CLOCK, 8, false);
	clock.sprite->addKeyframe(IDLE_CLOCK, glm::vec2(1 / 8.f * 0, 1 / 8.f * 3));
	clock.sprite->addKeyframe(IDLE_CLOCK, glm::vec2(1 / 8.f * 1, 1 / 8.f * 3));
	clock.sprite->addKeyframe(IDLE_CLOCK, glm::vec2(1 / 8.f * 2, 1 / 8.f * 3));
	clock.sprite->addKeyframe(IDLE_CLOCK, glm::vec2(1 / 8.f * 3, 1 / 8.f * 3));
	clock.sprite->addKeyframe(IDLE_CLOCK, glm::vec2(1 / 8.f * 4, 1 / 8.f * 3));
	clock.sprite->addKeyframe(IDLE_CLOCK, glm::vec2(1 / 8.f * 5, 1 / 8.f * 3));
	clock.sprite->addKeyframe(IDLE_CLOCK, glm::vec2(1 / 8.f * 6, 1 / 8.f * 3));
	clock.sprite->addKeyframe(IDLE_CLOCK, glm::vec2(1 / 8.f * 7, 1 / 8.f * 3));
	
	clock.sprite->changeAnimation(IDLE_CLOCK);

	clock.sprite->addEffect(EFFECT_SIN_Y, 120 * 1000);

	return clock;
}


void Scene::spawnDoorParticle(glm::vec2 pos) {
	particleDoor = Sprite::createSprite(glm::ivec2(SCREEN_X, SCREEN_Y), glm::vec2(32, 64), glm::vec2(1 / 16.f, 1 / 8.f), &particleSpritesheet, &texProgram);
	particleDoor->setDisplacement(glm::vec2(1 / 16.f * 0, 0.0f));
	particleDoor->setPosition(glm::ivec2(pos.x, pos.y));
	particleDoor->setNumberAnimations(1);
	particleDoor->setAnimationParams(DOOR_PARTICLE, 8, false);
	particleDoor->addKeyframe(DOOR_PARTICLE, glm::vec2(1 / 16.f * 0, 0.0f));
	particleDoor->addKeyframe(DOOR_PARTICLE, glm::vec2(1 / 16.f * 1, 0.0f));
	particleDoor->addKeyframe(DOOR_PARTICLE, glm::vec2(1 / 16.f * 2, 0.0f));
	particleDoor->addKeyframe(DOOR_PARTICLE, glm::vec2(1 / 16.f * 3, 0.0f));
	particleDoor->addKeyframe(DOOR_PARTICLE, glm::vec2(1 / 16.f * 4, 0.0f));
	particleDoor->addKeyframe(DOOR_PARTICLE, glm::vec2(1 / 16.f * 5, 0.0f));
	particleDoor->addKeyframe(DOOR_PARTICLE, glm::vec2(1 / 16.f * 6, 0.0f));
	particleDoor->addKeyframe(DOOR_PARTICLE, glm::vec2(1 / 16.f * 7, 0.0f));
	particleDoor->addKeyframe(DOOR_PARTICLE, glm::vec2(1 / 16.f * 8, 0.0f));

	particleDoor->changeAnimation(DOOR_PARTICLE);
}

void Scene::spawnCor()
{
	cor = Sprite::createSprite(glm::ivec2(SCREEN_X, SCREEN_Y), glm::vec2(27, 27), glm::vec2(1 / 8.f, 1/1.f), &corSpritesheet, &texProgram);
	cor->setDisplacement(glm::vec2(1 / 8.f * 0, 0.0f));
	cor->setPosition(glm::ivec2(0, -1.65 * map->getTileSize()));
	cor->setNumberAnimations(4);
	cor->setAnimationParams(COR_FULL, 1, false);
	cor->addKeyframe(COR_FULL, glm::vec2(0, 0.0f));
	cor->setAnimationParams(COR_75, 1, false);
	cor->addKeyframe(COR_75, glm::vec2(1 / 8.f * 1, 0.0f));
	cor->setAnimationParams(COR_50, 1, false);
	cor->addKeyframe(COR_50, glm::vec2(1 / 8.f * 2, 0.0f));
	cor->setAnimationParams(COR_POCHO, 1, false);
	cor->addKeyframe(COR_POCHO, glm::vec2(1 / 8.f * 3, 0.0f));
	cor->changeAnimation(COR_FULL);
}
