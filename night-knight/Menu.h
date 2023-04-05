#pragma once

#include "ShaderProgram.h"
#include "Text.h"
#include "Scene.h"

enum Scenes {
	MAIN_MENU, HOW_TO_PLAY1, HOW_TO_PLAY2, HOW_TO_PLAY3, HOW_TO_PLAY4, CREDITS
};

struct TextInfo {
	string text;
	glm::ivec2 pos;
	int textSize;
	int alignment;
};

class Menu
{
public:
	Menu();
	~Menu();

	void init();
	void update(int deltaTime);
	void render();

private:
	void initShaders();
	void moveBetweenHowToPlay();
	void spawnDoor(int scene);
	void spawnKey (int scene);
	void spawnPlatforms(int scene);
	void spawnMan(int scene);

	Texture bgSpritesheet, bgcreditsheet, bghowtoplaysheet, doorSpritesheet, itemsSpriteSheet, platformsSpritesheet;
	Texture manSpriteSheet;

	Sprite* bg;
	Sprite* bgCredit;
	Sprite* bghowtoplay;

	//Cada elemento del vector es el vector de cada escena
	vector<vector<Sprite*>> images;
	vector<vector<TextInfo>> texts;

	Text titleObj;
	Text textObj;

	ShaderProgram texProgram;
	glm::mat4 projection;
	int scene;
	int selected;

	ISoundEngine* engine;
	ISoundSource* menuNavSrc;
};

