#pragma once

#include "ShaderProgram.h"
#include "Text.h"

enum Scenes {
	MAIN_MENU, CREDITS, HOW_TO_PLAY
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

	Text text;
	ShaderProgram texProgram;

	int scene;
};

