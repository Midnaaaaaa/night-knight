#pragma once

#include "ShaderProgram.h"
#include "Text.h"
#include "Scene.h"

enum Scenes {
	MAIN_MENU, CREDITS, HOW_TO_PLAY
};

class Menu : public Scene
{
public:
	Menu();
	~Menu();

	virtual void init();
	virtual void update(int deltaTime);
	virtual void render();


private:
	void initShaders();

	vector<Sprite*> images;
	vector<pair<string, glm::ivec2>> texts;

	Text titleObj;
	Text textObj;

	ShaderProgram texProgram;

	int scene;
};




