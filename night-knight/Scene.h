#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE

#include <queue>
#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"
#include "Enemy.h"
#include "Text.h"
#include "Item.h"

// Scene contains all the entities of our game.
// It is responsible for updating and render them.


class Scene
{

public:
	Scene(int level);
	~Scene();

	void init();
	void update(int deltaTime);
	void render();

private:
	void initShaders();
	void spawnKey();
	void spawnDoor();
	Item spawnHourglass(glm::vec2 pos);
	Item spawnGem(glm::vec2 pos);
	Item spawnClock(glm::vec2 pos);
	void spawnDoorParticle(glm::vec2 pos);
	void spawnCor();
	//void executeFunction(void(Scene::*)());

	//void gameOver();

private:
	int level;

	Texture bgSpritesheet, objectsSpritesheet, doorSpritesheet, particleSpritesheet, corSpritesheet;

	Sprite *bg;
	TileMap *map;
	Player *player;
	vector<Enemy*> enemies;
	ShaderProgram texProgram;
	float currentTime;
	glm::mat4 projection;

	Enemy* asesino;

	Sprite* key;
	Sprite* door;
	Sprite* particleDoor;
	Sprite* cor;

	glm::vec2 doorPos, keyPos, initPlayerPos;


	bool keyCollected;
	int spawnTimer = -1, despawnTimer = -1;
	deque<Item> objects; 
	int stageTimer, gameOverTimer, stageCompletedTimer;
	bool gameOver, stageCompleted;
	//void(Scene::*timerFunc)();

	Text text;
};


#endif // _SCENE_INCLUDE

