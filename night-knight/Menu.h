#pragma once

#include "ShaderProgram.h"
#include "Text.h"
#include "Scene.h"

enum Scenes {
	MAIN_MENU, HOW_TO_PLAY1, HOW_TO_PLAY2, HOW_TO_PLAY3, HOW_TO_PLAY4, CREDITS
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

	Texture bgSpritesheet, sirbernardsheet, bgcreditsheet;

	Sprite* bg;
	Sprite* bgCredit;
	Sprite* sirbernard;

	//Cada elemento del vector es el vector de cada escena
	vector<vector<Sprite*>> images;
	vector<vector<pair<string, glm::ivec2>>> texts;

	Text titleObj;
	Text textObj;

	ShaderProgram texProgram;
	glm::mat4 projection;
	int scene;
	int selected;

	ISoundEngine* engine;
	ISoundSource* menuNavSrc;
};

