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
	Scene();
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
	//void executeFunction(void(Scene::*)());

	//void gameOver();

private:
	Texture bgSpritesheet, objectsSpritesheet, doorSpritesheet;

	Sprite *bg;
	TileMap *map;
	Player *player;
	vector<Enemy*> enemies;
	ShaderProgram texProgram;
	float currentTime;
	glm::mat4 projection;

	Sprite* key;
	Sprite* door;


	bool keyCollected;
	int spawnTimer = -1, despawnTimer = -1;
	deque<Item> objects; 
	int timer, pauseTimer, gameOverTimer;
	bool gameOver;
	void(Scene::*timerFunc)();

	Text text;
};


#endif // _SCENE_INCLUDE

