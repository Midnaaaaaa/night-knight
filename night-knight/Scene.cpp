#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"
#include "Esquelet.h"
#include "Vampir.h"
#include "Fantasma.h"


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

	map = TileMap::createTileMap("levels/level28.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	objectsSpritesheet.loadFromFile("images/varied.png", TEXTURE_PIXEL_FORMAT_RGBA);
	bgSpritesheet.loadFromFile("images/bg28.png", TEXTURE_PIXEL_FORMAT_RGBA);
	bg = Sprite::createSprite(glm::ivec2(32*map->getTileSize(), 22*map->getTileSize()), glm::ivec2(1,1), &bgSpritesheet, &texProgram);
	bg->setPosition(glm::ivec2(SCREEN_X, SCREEN_Y));
	

	//Enemigos
	Vampir *prueba = new Vampir();
	prueba->init(glm::ivec2(SCREEN_X, SCREEN_Y), false, "images/bub.png", glm::ivec2(32, 32), glm::ivec2(0, 0), glm::ivec2(32, 32), glm::vec2(1 / 4.f, 1 / 4.f), texProgram);
	prueba->setPosition(glm::vec2(10 * map->getTileSize(), 3 * map->getTileSize()));
	prueba->setTileMap(map);
	enemies.push_back(prueba);

	
	Fantasma* prueba1 = new Fantasma();
	prueba1->init(glm::ivec2(SCREEN_X, SCREEN_Y), true, "images/bub.png",  glm::ivec2(32, 32), glm::ivec2(0, 0), glm::ivec2(32, 32), glm::vec2(1 / 4.f, 1 / 4.f), texProgram);
	prueba1->setPosition(glm::vec2(10 * map->getTileSize(), 6 * map->getTileSize()));
	prueba1->setTileMap(map);
	enemies.push_back(prueba1);
	

	//Player
	player = new Player();
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), true, "images/soma-animations.png", glm::ivec2(16,32), glm::ivec2(8,32), glm::ivec2(32,64), glm::vec2(1/16.f, 1/16.f), texProgram);

	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
	player->setTileMap(map);
	player->setSpeed(2);


	//Objetos (sprites)
	key = nullptr;

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
	if (currentTime > 5000 && key == nullptr) {
		spawnKey();
	}
}

void Scene::render()
{
	bg->render();


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

	if (key != nullptr) {
		key->render();
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

void Scene::spawnKey() {
	key = Sprite::createSprite(glm::vec2(32, 32), glm::vec2(1/2.f, 1/2.f), &objectsSpritesheet, &texProgram);
	key->setDisplacement(glm::vec2(1 / 2.f, 1 / 2.f));
	key->setPosition(glm::ivec2(SCREEN_X + 28 * map->getTileSize(), SCREEN_Y + 18 * map->getTileSize()));
}

