#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"
#include "Enemy.h"
#include "Text.h"

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

	int timer, pauseTimer, gameOverTimer;
	bool gameOver;
	void(Scene::*timerFunc)();

	Text text;
};


#endif // _SCENE_INCLUDE

