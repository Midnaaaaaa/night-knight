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
#include "SoundManager.h"

#define MAX_LIGHTS 10


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
	void changePauseState();
	bool getPauseState();



private:
	void initShaders();
	void spawnKey();
	void spawnDoor();
	Item spawnHourglass(glm::vec2 pos);
	Item spawnGem(glm::vec2 pos);
	Item spawnClock(glm::vec2 pos);
	void spawnDoorParticle(glm::vec2 pos);
	void spawnCor();
	void spawnTorch(glm::ivec2 pos, int type);
	//void executeFunction(void(Scene::*)());
	vector<pair<glm::ivec2, int>>* getLightSources();

	//void gameOver();

	bool loadLevelInfo(const string& levelFile);

private:
	void updateTimers(int deltaTime);

	int level;

	Texture bgSpritesheet, objectsSpritesheet, doorSpritesheet, particleSpritesheet, corSpritesheet, torchSpriteSheet;

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

	vector<Sprite*> torches;

	glm::vec2 doorPos, keyPos, initPlayerPos;


	bool keyCollected;
	int spawnTimer = -1, despawnTimer = -1, freezeTimer, startTimer;
	deque<Item> objects; 
	int stageTimer, gameOverTimer, stageCompletedTimer;
	bool gameOver, stageCompleted;
	//void(Scene::*timerFunc)();

	Text text;

	ISoundEngine* engine;
	ISound* bgSound;
	ISoundSource* puntIncrSrc;

	ISoundSource* hourglassSrc, * gemSoundSrc, * clockSoundSrc;

	ISound* puntIncrSound;
	ISound* hourglassSound;
	ISound* gameOverSound;
	ISound* readySound;
	ISound* clockSound;

	int tickSoundTime;


	bool paused;

	vector<pair<glm::ivec2, int>> lightSources;


};


#endif // _SCENE_INCLUDE

