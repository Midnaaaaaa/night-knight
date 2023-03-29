#include "Menu.h"
#include <iostream>

Menu::Menu()
{
}

Menu::~Menu()
{
}

void Menu::init()
{
}

void Menu::update(int deltaTime)
{
}

void Menu::render()
{
	for (Sprite* s : images)
	{
		s->render();
	}
	for (pair<string, glm::ivec2> text : texts)
	{
		textObj.render(text.first, text.second, 16, glm::vec4(1, 1, 1, 1));
	}
	titleObj.render("Night Knight", glm::ivec2(0, 0), 32, glm::vec4(0.7, 0.2, 0.1, 1));
}

void Menu::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if (!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if (!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	texProgram.init();
	texProgram.addShader(vShader);
	texProgram.addShader(fShader);
	texProgram.link();
	if (!texProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << texProgram.log() << endl << endl;
	}
	texProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
}
