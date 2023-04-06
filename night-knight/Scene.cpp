#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"
#include "Esquelet.h"
#include "Vampir.h"
#include "Fantasma.h"
#include <iomanip>

#include <fstream>
#include <sstream>

#define MIN_TIME_WITHOUT_SPAWN 8  * 1000
#define MAX_TIME_WITHOUT_SPAWN 15 * 1000

#define MIN_TIME_TO_DESPAWN 10  * 1000
#define MAX_TIME_TO_DESPAWN 15 * 1000

#define OBJ_SPAWN_ANIM_TIME 1000

#define SCREEN_X 32*2
#define SCREEN_Y 16*2

#define STAGE_TIMER 60000

#define START_TIME 4000

#define LEVELS_PER_GROUP 2



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

enum EnemyType {
	ESQUELET, VAMPIR, FANTASMA
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


	engine->removeSoundSource(puntIncrSrc);
	engine->removeSoundSource(hourglassSrc);
	engine->removeSoundSource(gemSoundSrc);
	engine->removeSoundSource(clockSoundSrc);
	SoundManager::instance().stopBgMusic();

	if (readySound != nullptr) {
		readySound->stop();
		readySound->drop();
	}
}


void Scene::init()
{
	stageTimer = STAGE_TIMER;
	stageCompleted = false;
	stageCompletedTimer = 5000;
	gameOverTimer = 7000;
	freezeTimer = 0;
	paused = false;

	initShaders();

	loadLevelInfo("levels/level" + to_string(level) + ".txt");


	objectsSpritesheet.loadFromFile("images/items.png", TEXTURE_PIXEL_FORMAT_RGBA);
	bgSpritesheet.loadFromFile("images/bg" + to_string(level) + ".png", TEXTURE_PIXEL_FORMAT_RGBA);
	doorSpritesheet.loadFromFile("images/door"+ to_string((level-1)/LEVELS_PER_GROUP + 1) + ".png", TEXTURE_PIXEL_FORMAT_RGBA);
	particleSpritesheet.loadFromFile("images/particles.png", TEXTURE_PIXEL_FORMAT_RGBA);
	corSpritesheet.loadFromFile("images/cor.png", TEXTURE_PIXEL_FORMAT_RGBA);
	torchSpriteSheet.loadFromFile("images/torch.png", TEXTURE_PIXEL_FORMAT_RGBA);

	bg = Sprite::createSprite(glm::ivec2(SCREEN_X, SCREEN_Y), glm::ivec2(32*map->getTileSize(), 22*map->getTileSize()), glm::ivec2(1,1), &bgSpritesheet, &texProgram);
	//bg->setPosition(glm::ivec2(SCREEN_X, SCREEN_Y));
	
	/*
	initPlayerPos = glm::vec2(2, 16);
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
	

	//player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
	//player->setSpeed(2);

	//Objetos (sprites)
	*/
	spawnCor();

	projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);
	currentTime = 0.0f;

	keyCollected = false;

	// Select which font you want to use
	if (!text.init("fonts/ArcadeClassic.ttf", glm::ivec2(SCREEN_X, SCREEN_Y), projection))
		//if(!text.init("fonts/OpenSans-Bold.ttf"))
		//if(!text.init("fonts/DroidSerif.ttf"))
		cout << "Could not load font!!!" << endl;

	engine = SoundManager::instance().getSoundEngine();

	puntIncrSrc = engine->addSoundSourceFromFile("sound/bit.wav");
	puntIncrSrc->setDefaultVolume(0.4);

	hourglassSrc = engine->addSoundSourceFromFile("sound/hourglass.mp3");
	gemSoundSrc = engine->addSoundSourceFromFile("sound/gem.wav");
	clockSoundSrc = engine->addSoundSourceFromFile("sound/clock.mp3");

	startTimer = START_TIME;

}

bool Scene::loadLevelInfo(const string& levelFile) {
	ifstream fin;
	string line, tilesheetFile;
	stringstream sstream;
	short tile;

	fin.open(levelFile.c_str());
	if (!fin.is_open())
		return false;
	getline(fin, line);
	if (line.compare(0, 7, "TILEMAP") != 0)
		return false;

	string tileMapInfoStr = "";

	getline(fin, line);
	while (line.compare(0, 7, "TILEMAP") != 0) {
		tileMapInfoStr += line + "\n";
		getline(fin, line);
	}

	map = TileMap::createTileMap(tileMapInfoStr, glm::vec2(SCREEN_X, SCREEN_Y), texProgram);

	/**
	* Ahora se leen:
	* - Posicion de la puerta
	* - Posicion de la llave
	* - Posicion inicial del jugador
	* - Numero de enemigos
	* - info de cada enemigo
	* 
	* */

	// Leer la posición de la llave
	std::getline(fin, line);
	sstream.str(line);
	sstream >> keyPos.x >> keyPos.y;
	sstream.clear();
	key = nullptr;

	// Leer la posición de la puerta
	std::getline(fin, line);
	sstream.str(line);
	sstream >> doorPos.x >> doorPos.y;
	sstream.clear();
	spawnDoor();


	// Leer la posición inicial del jugador y info
	std::getline(fin, line);
	sstream.str(line);
	bool rightSight;
	sstream >> initPlayerPos.x >> initPlayerPos.y >> rightSight;
	sstream.clear();
	player = new Player();
	player->setTileMap(map);
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), rightSight, "images/soma-animations.png", glm::ivec2(16, 32), glm::ivec2(8, 32), glm::ivec2(32, 64), glm::vec2(1 / 16.f, 1 / 16.f), initPlayerPos, texProgram);
	player->increasePuntuacion(Game::instance().getPuntuacionActual());
	player->setVidas(Game::instance().getVidasActuales());
	// Leer el número de enemigos
	int numEnemies;
	std::getline(fin, line);
	sstream.str(line);
	sstream >> numEnemies;
	sstream.clear();

	// Leer la posición de cada enemigo
	for (int i = 0; i < numEnemies; i++) {
		std::getline(fin, line);
		sstream.str(line);
		int enemyType;
		glm::ivec2 pos;
		sstream >> enemyType >> pos.x >> pos.y >> rightSight;
		sstream.clear();

		Enemy* enemy;
		switch (enemyType)
		{
		case ESQUELET:
			enemy = new Esquelet();
			break;
		case VAMPIR:
			enemy = new Vampir();
			break;
		case FANTASMA:
		default:
			enemy = new Fantasma();
			break;
		}
		enemy->init(glm::ivec2(SCREEN_X, SCREEN_Y), rightSight, pos, map, texProgram);
		enemies.push_back(enemy);
	}

	// Leer el número de enemigos
	int numLights;
	std::getline(fin, line);
	sstream.str(line);
	sstream >> numLights;
	sstream.clear();

	// Leer la posición de cada enemigo
	for (int i = 0; i < numLights; i++) {
		std::getline(fin, line);
		sstream.str(line);
		int type;
		glm::ivec2 pos;
		sstream >> type >> pos.x >> pos.y;
		sstream.clear();
		spawnTorch(pos, type);
	}
	fin.close();

	return true;
}

void Scene::updateTimers(int deltaTime) {
	if (freezeTimer != 0) {
		freezeTimer -= deltaTime;
		if (freezeTimer <= 0) {
			freezeTimer = 0;
			if (hourglassSound != nullptr) {
				hourglassSound->stop();
				hourglassSound->drop();
				hourglassSound = nullptr;
			}
			bgSound->setVolume(1);
		}
	}

	if (startTimer != 0) {
		startTimer -= deltaTime;
		if (readySound == nullptr) {
			readySound = engine->play2D("sound/stageStart.mp3", false, false, true);
		}
		if (startTimer <= 0) {
			startTimer = 0;
			string levelSoundFile = "sound/lvl" + to_string((level-1) / LEVELS_PER_GROUP + 1) + ".mp3";
			bgSound = SoundManager::instance().changeBgMusic(levelSoundFile.c_str(), true, false);
			readySound->stop();
			readySound->drop();
			readySound = nullptr;
		}
	}
}

void Scene::update(int deltaTime)
{
	updateTimers(deltaTime);
	if (startTimer > 0) return;


	currentTime += deltaTime;

	//Check game state
	if (player->isGameOver() || stageTimer <= 0 && player->getVidas() == 1) {
		gameOver = true;
		SoundManager::instance().pauseBgMusic(true);
	}
	else if (stageTimer <= 0) {
		player->muelto();
		stageTimer = STAGE_TIMER;
	}

	door->update(deltaTime);
	//Colision puerta
	if (door->animation() == DOOR_OPENED && !stageCompleted) {
		glm::vec2 topLeft = door->getPosition();
		glm::vec2 bottomRight = topLeft + door->getSpriteSize();
		if (player->checkCollisionWithRect(topLeft, bottomRight, 2)) {
			stageCompleted = true;
			glm::vec2 pos = door->getPosition();
			player->addEffect(EFFECT_INVIS, 60000);
			glm::ivec2 doorPos = door->getPosition() + door->getSpriteSize()*0.5f;
			player->addEffect(5, 2000, doorPos);
			player->setSpeed(0);
			spawnDoorParticle(pos);
			SoundManager::instance().pauseBgMusic(true);
		}
	}

	if (stageCompleted) {
		if (clockSound != nullptr) {
			clockSound->stop();
			clockSound->drop();
			clockSound = nullptr;
		}
		if (hourglassSound != nullptr) {
			hourglassSound->stop();
			hourglassSound->drop();
			hourglassSound = nullptr;
		}
		stageCompletedTimer -= deltaTime;
		player->update(deltaTime);
		if (particleDoor != nullptr) particleDoor->update(deltaTime);
		if (stageCompletedTimer <= 4500 && enemies.size() > 0) {
			for (int i = enemies.size() - 1; i >= 0; --i)
			{
				delete enemies[i];
				enemies.pop_back();
			}
			if (level == 8) engine->play2D("sound/clearlvl8.ogg");
			else engine->play2D("sound/stageClear.ogg");
		}
		if (stageTimer / 1000 > 0 && stageCompletedTimer <= 1000) {
			int stageTimerActual = stageTimer / 1000;
			stageTimer -= deltaTime * 20;
			player->increasePuntuacion((stageTimerActual - stageTimer/1000)*10);
			
			if (puntIncrSound == nullptr) {
				puntIncrSound = engine->play2D(puntIncrSrc, true, false, true);
			}
			stageCompletedTimer = 1000;
		}
		else if (stageTimer / 1000 <= 0) {
			if (puntIncrSound != nullptr) {
				puntIncrSound->stop();
				puntIncrSound->drop();
				puntIncrSound = nullptr;
			}
			if (stageCompletedTimer <= 0 && level != 8 || stageCompletedTimer <= -2500) {
				Game::instance().savePuntuacionYVidas(player->getPuntuacion(), player->getVidas());
				Game::instance().changeLevel(level + 1);
			}
		}
		return;
	}
	else if (gameOver) {
		if (clockSound != nullptr) {
			clockSound->stop();
			clockSound->drop();
			clockSound = nullptr;
		}
		if (hourglassSound != nullptr) {
			hourglassSound->stop();
			hourglassSound->drop();
			hourglassSound = nullptr;
		}
		if (player->sePuedeMover()) player->setSpeed(0);
		gameOverTimer -= deltaTime;
		player->update(deltaTime);
		if (gameOverTimer <= 4000 && gameOverSound == nullptr) {
			gameOverSound = engine->play2D("sound/gameOver.mp3", false, false, true);
		}
		else if (gameOverTimer <= 0) {
			gameOverSound->stop();
			gameOverSound->drop();
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
		Item item;
		switch (objectToSpawn)
		{
		case HOURGLASS:
			item = spawnHourglass(glm::vec2(platform.x + 0.5, platform.y - 1.5));
			break;
		case GEM:
			item = spawnGem(glm::vec2(platform.x + 0.5, platform.y - 1.5));
			break;
		case CLOCK:
			item = spawnClock(glm::vec2(platform.x + 0.5, platform.y - 1.5));
			break;
		}
		glm::ivec2 center = item.sprite->getPosition() + item.sprite->getSpriteSize() * 0.5f;
		item.sprite->addEffect(EFFECT_BH, -OBJ_SPAWN_ANIM_TIME, center);
		objects.push_back(item);
		if (despawnTimer == -1) {
			despawnTimer = rand() % (MAX_TIME_TO_DESPAWN - MIN_TIME_TO_DESPAWN + 1) + MIN_TIME_TO_DESPAWN;
			despawnTimer += currentTime;
		}
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
	else if (currentTime >= despawnTimer - OBJ_SPAWN_ANIM_TIME && despawnTimer != -1) {
		if (!objects.empty()) {
			Item obj;
			obj = objects.front();
			if (obj.sprite->currentEffectId() != EFFECT_BH && obj.sprite->currentEffectId() != EFFECT_INVIS) {
				glm::ivec2 center = obj.sprite->getPosition() + obj.sprite->getSpriteSize() * 0.5f;
				obj.sprite->addEffect(EFFECT_INVIS, 3000);
				obj.sprite->addEffect(EFFECT_BH, OBJ_SPAWN_ANIM_TIME, center);
			}
		}
	}

	if (Game::instance().getKeyUp('g')) {
		player->setGodMode(!player->inGodMode());
	}


	if ((Game::instance().getKeyUp('k') || map->getNumOfTilesRemaining() == 0) && !keyCollected && key == nullptr) {
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
			engine->play2D("sound/door.ogg");
		}
	}

	//Collision with items
	for (std::deque<Item>::iterator it = objects.begin(); it != objects.end(); ++it) {
		it->sprite->update(deltaTime);
		glm::vec2 topLeft = it->sprite->getPosition();
		glm::vec2 bottomRight = topLeft + it->sprite->getSpriteSize();
		if (player->checkCollisionWithRect(topLeft, bottomRight, 2)) {
			switch (it->id)
			{
			case GEM:
				player->increasePuntuacion(1000);
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
					hourglassSound = engine->play2D("sound/hourglass.mp3", true, false, true);
				}
				else {
					hourglassSound->setPlayPosition(0);
					hourglassSound->setIsPaused(false);
				}
				bgSound->setVolume(0.4);
				break;
			case CLOCK:
				stageTimer += 15000;
				if (clockSound == nullptr || clockSound->isFinished()) {
					clockSound = engine->play2D(clockSoundSrc, true, false, true);
				}
				else {
					clockSound->setPlayPosition(0);
					clockSound->setIsPaused(false);
				}
				tickSoundTime = currentTime + 2000;
				break;
			}
			it->sprite->free();
			it = objects.erase(it);
			if (it == objects.end()) break;
		}
	}

	if (currentTime <= tickSoundTime && clockSound != nullptr) {
		float ratio = 1-((tickSoundTime - currentTime)/2000) + 1;
		clockSound->setPlaybackSpeed(ratio);
	}
	else if (clockSound != nullptr) {
		clockSound->stop();
		clockSound->drop();
		clockSound = nullptr;
	}
;
	for (Sprite* s : torches)
	{
		s->update(deltaTime);
	}

	asesino = nullptr;
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

	//UPDATE TIMERS
	//if (timer >= currentTime) {
	//	(this->*timerFunc)();
	//}

	//if (player->isGameOver()) {
	//	timer = 4000;
	//	timerFunc = gameOver;
	//}
	
}

vector<pair<glm::ivec2, int>>* Scene::getLightSources()
{
	lightSources.resize(enemies.size() + torches.size() + 1);
	lightSources[0].first = player->getCenterPos() + glm::ivec2(SCREEN_X, SCREEN_Y);

	float PI = glm::pi<float>();
	float sino = glm::sin((currentTime/200) * 2 * PI);
	lightSources[0].second = 75 + (sino * 2);

	int k = 1;
	for (int i = 0; i < enemies.size() && k < MAX_LIGHTS; ++i) {
		lightSources[k].first = enemies[i]->getCenterPos() + glm::ivec2(SCREEN_X, SCREEN_Y);
		lightSources[k].second = 45 + sino * 2;
		++k;
	}
	for (int i = 0; i < torches.size() && k < MAX_LIGHTS; ++i) {
		lightSources[k].first = torches[i]->getPosition() + torches[i]->getSpriteSize()*0.5f + glm::vec2(SCREEN_X, SCREEN_Y);
		lightSources[k].second = 45 + sino * 2;
		++k;
	}

	pair<glm::ivec2, int> keyData;

	if (key != nullptr) {
		keyData.first = key->getPosition() + key->getSpriteSize()*0.5f + glm::vec2(SCREEN_X, SCREEN_Y);
		keyData.second = 32;

		lightSources.push_back(keyData);
	}

	return &lightSources;
}


void Scene::render()
{
	//Render de game over

	if (gameOver) {
		player->render();
		
		if(stageTimer <= 0) text.render(to_string(stageTimer / 1000), glm::vec2(SCREEN_WIDTH / 2, 30.f), 32, glm::vec4(0.7, 0.2, 0.1, 1), Text::CENTERED);
		else if (asesino != nullptr) asesino->render();
		if (gameOverTimer <= 4000) {
			text.render("GAME    OVER", glm::vec2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2), 32, glm::vec4(1, 1, 1, 1), Text::CENTERED);
		}
		return;
	}

	//La misma camara ortogonal para TODOS
	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);


	if (level == 7 || level == 8) { //Dark
		vector<pair<glm::ivec2, int>>* data = getLightSources();
		int n = data->size();

		float lightPos[MAX_LIGHTS * 2];
		int radius[MAX_LIGHTS];
		for (int i = 0; i < n; ++i) {
			lightPos[2*i] = (*data)[i].first.x;
			lightPos[2*i + 1] = (*data)[i].first.y;
			radius[i] = (*data)[i].second;
		}

		glm::vec4 aux =  projection* glm::vec4(lightPos[0], lightPos[1], 0, 1);

		texProgram.setUniform1iv("radius", n, radius);
		texProgram.setUniform2fv("center", n, lightPos);
		texProgram.setUniform1i("count", n);

		int vp[4];
		glGetIntegerv(GL_VIEWPORT, vp);
		texProgram.setUniform1i("WIDTH", vp[2]);
		texProgram.setUniform1i("HEIGHT", vp[3]);
	}

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

	for (Sprite* s : torches)
	{
		s->render();
	}

	player->render();
	if (particleDoor != nullptr) {
		particleDoor->render();
	}

	for (Enemy* e : enemies)
	{
		e->render();
	}


	for (Item i : objects)
	{
		i.sprite->render();
	}

	if (key != nullptr) {
		key->render();
	}


	//Disable dark
	texProgram.setUniform1i("count", 0);
	cor->render();
	//Render de num vidas
	text.render("x" + to_string(player->getVidas()), glm::vec2(SCREEN_X + 26, 30.f), 26, glm::vec4(1, 1, 1, 1));

	//Render de puntuacion
	stringstream ss;

	ss << setw(6) << setfill('0') << player->getPuntuacion();
	text.render(ss.str(), glm::vec2(150.f, 30.f), 26, glm::vec4(1, 1, 1, 1));


	if (stageTimer < 20000 && stageTimer > 5000 && !stageCompleted) {
		text.render(to_string(stageTimer / 1000), glm::vec2(SCREEN_WIDTH / 2, 30.f), 32, glm::vec4(1, 1, 0, 1), Text::CENTERED);
	}
	else if (stageTimer < 5000 && !stageCompleted){
		text.render(to_string(stageTimer / 1000), glm::vec2(SCREEN_WIDTH / 2, 30.f), 32, glm::vec4(0.7, 0.2, 0.1, 1), Text::CENTERED);
	}
	else {
		text.render(to_string(stageTimer / 1000), glm::vec2(SCREEN_WIDTH / 2, 30.f), 32, glm::vec4(1, 1, 1, 1), Text::CENTERED);
	}
	if (paused) {
		text.render("PAUSED", glm::vec2(SCREEN_WIDTH/2, SCREEN_HEIGHT/2), 26, glm::vec4(1, 1, 1, 1), Text::CENTERED);

	}

	if (stageCompletedTimer <= 4500 && stageCompleted) {
		text.render("STAGE    CLEAR", glm::vec2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2), 26, glm::vec4(1, 1, 1, 1), Text::CENTERED);
	}

	text.render("STAGE " + to_string(level), glm::vec2(SCREEN_X + map->getTileSize() * 32, 30.f), 26, glm::vec4(1, 1, 1, 1), Text::RIGHT_ALIGNED);
	
	if (startTimer > 500) {
		text.render("READY", glm::vec2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2), 26, glm::vec4(1, 1, 1, 1), Text::CENTERED);
	}
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
	glm::ivec2 center = key->getPosition() + key->getSpriteSize() * 0.5f;
	key->addEffect(EFFECT_BH, -1000, center);

	ISound* is = engine->play2D("sound/keys.ogg", false, false, true);
	is->setVolume(2);
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
	particleDoor->setPosition(glm::ivec2(pos.x, pos.y - 32));
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

void Scene::spawnTorch(glm::ivec2 pos, int type)
{
	Sprite* light = Sprite::createSprite(glm::ivec2(SCREEN_X, SCREEN_Y), glm::vec2(16, 32), glm::vec2(1 / 4.f, 1 / 4.f), &torchSpriteSheet, &texProgram);
	light->setPosition(pos * map->getTileSize());
	light->setNumberAnimations(1);
	light->setAnimationParams(0, 10, false);
	light->addKeyframe(0, glm::vec2(1 / 4.f * 0, 1 / 4.f * type));
	light->addKeyframe(0, glm::vec2(1 / 4.f * 1, 1 / 4.f * type));
	light->addKeyframe(0, glm::vec2(1 / 4.f * 2, 1 / 4.f * type));
	light->addKeyframe(0, glm::vec2(1 / 4.f * 3, 1 / 4.f * type));
	light->changeAnimation(0);
	torches.push_back(light);

}

void Scene::changePauseState() {
	paused = !paused;
}

bool Scene::getPauseState() {
	return paused;
}
