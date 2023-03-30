#include "Menu.h"
#include <iostream>
#include "Game.h"
#include <GL/glut.h>
#include <glm/gtc/matrix_transform.hpp>


#define SCREEN_X 32*2
#define SCREEN_Y 16*2

Menu::Menu()
{
	scene = 0;
	selected = 0;
}

Menu::~Menu()
{
}

void Menu::init()
{
	textObj.init("fonts/ArcadeClassic.ttf", glm::ivec2(SCREEN_X, SCREEN_Y));
	titleObj.init("fonts/AncientModernTales.otf", glm::ivec2(SCREEN_X, SCREEN_Y));
	//Harcoded :+1:

	//vector<pair<string, glm::ivec2>>
	//EJEMPLO:
	texts.push_back({ {"Play", glm::ivec2(0,0)}, {"How to play", glm::ivec2(0,10)} });

}

//Hacemos que devuelva un int o un bool para indicar que quiere cambiar al primer nivel?
void Menu::update(int deltaTime)
{
	projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);

	if (scene == MAIN_MENU) {
		if (Game::instance().getSpecialKey(GLUT_KEY_UP)) {
			if (--selected < 0) selected = 3;
		}
		else if (Game::instance().getSpecialKey(GLUT_KEY_DOWN)) {
			selected = selected % 4;
		}

		//seleccionar seleccion
		if (Game::instance().getKey('\r') || Game::instance().getKey(10) || Game::instance().getKey(' ')) {
			switch (selected)
			{
			case 0: //Jugar
				//return algo?
				break;
			case 1: //How to play?
				scene = HOW_TO_PLAY;
				break;
			case 2: //Creditos
				scene = CREDITS;
				break;
			case 3: //Salir
				//Salir
				break;
			}
		}

		if (Game::instance().getKey(27)) {
			exit(0);
		}
	}
	else { //Esc, o otra tecla, para volver
		if (Game::instance().getKey(27)) {
			scene = MAIN_MENU;
		}
	}
}

void Menu::render()
{
	//for (Sprite* s : images[scene])
	//{
	//	s->render();
	//}
	for (pair<string, glm::ivec2> text : texts[scene])
	{
		textObj.render(text.first, text.second, 16, glm::vec4(1, 1, 1, 1));
	}

	if (scene == MAIN_MENU) {
		titleObj.render("Night Knight", glm::ivec2(0, 0), 32, glm::vec4(0.7, 0.2, 0.1, 1));

		//Renderizar "flecha". Su posicion depende de la variable "selected"
	}
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
