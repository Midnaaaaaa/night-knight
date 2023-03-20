#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"


#define SCREEN_X 32*2
#define SCREEN_Y 16*2

#define INIT_PLAYER_X_TILES 2
#define INIT_PLAYER_Y_TILES 9


Scene::Scene()
{
	map = NULL;
	player = NULL;
}

Scene::~Scene()
{
	if(map != NULL)
		delete map;
	if(player != NULL)
		delete player;
}


void Scene::init()
{

	initShaders();
	map = TileMap::createTileMap("levels/level01.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	player = new Player();
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), "images/simon-sprite.png", glm::ivec2(16,32), glm::ivec2(8,0), glm::ivec2(32,32), glm::vec2(1/8.f, 1/4.f), texProgram);

	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
	player->setTileMap(map);

	Esquelet *prueba = new Esquelet();
	prueba->init(glm::ivec2(SCREEN_X, SCREEN_Y), "images/bub.png", glm::ivec2(32, 32), glm::ivec2(0, 0), glm::ivec2(32, 32), glm::vec2(1 / 4.f, 1 / 4.f), texProgram);
	prueba->setPosition(glm::vec2(10 * map->getTileSize(), 3 * map->getTileSize()));
	prueba->setTileMap(map);
	enemies.push_back(prueba);
	projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);
	currentTime = 0.0f;
}

void Scene::update(int deltaTime)
{
	currentTime += deltaTime;
	player->update(deltaTime);
	for (Enemy* e : enemies)
	{
		e->update(deltaTime);
		if (!player->isHurted()) {
			glm::ivec2 topLeft = e->getPosition();
			glm::ivec2 bottomRight = topLeft + e->getSize();
			//0 means TYPE_ENEMY
			bool wasHit = player->checkCollisionWithSprite(topLeft, bottomRight, 0);	
		}
	}
}

void Scene::render()
{
	glm::mat4 modelview;

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	map->render();
	player->render();
	for (Enemy* e : enemies)
	{
		e->render();
	}
}

void Scene::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if(!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if(!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	texProgram.init();
	texProgram.addShader(vShader);
	texProgram.addShader(fShader);
	texProgram.link();
	if(!texProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << texProgram.log() << endl << endl;
	}
	texProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
}



