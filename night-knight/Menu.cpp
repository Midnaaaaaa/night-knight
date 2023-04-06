#include "Menu.h"
#include <iostream>
#include "Game.h"
#include <GL/glut.h>
#include <glm/gtc/matrix_transform.hpp>


Menu::Menu()
{
	scene = 0;
	selected = 0;
}

Menu::~Menu()
{
	//menuNavSrc->drop();
}

void Menu::init()
{
	projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);
	textObj.init("fonts/PixelFont.ttf", glm::ivec2(0, 0), projection);
	titleObj.init("fonts/AncientModernTales.otf", glm::ivec2(0, 0), projection);
	//Harcoded :+1:

	initShaders();

	//vector<pair<string, glm::ivec2>>
	//EJEMPLO:

	texts.resize(8);
	images.resize(8);

	texts[MAIN_MENU] = { {"PLAY", glm::ivec2(SCREEN_WIDTH/2, SCREEN_HEIGHT/2) + 1, 16, Text::CENTERED},
		{"HOW TO PLAY", glm::ivec2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 50), 16, Text::CENTERED},
		{"CREDITS", glm::ivec2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 100), 16, Text::CENTERED},
		{"EXIT", glm::ivec2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 150), 16, Text::CENTERED} 
	};

	texts[CREDITS] = 
	{
		{"DEVELOPED BY", glm::ivec2(SCREEN_WIDTH / 2, 75), 16, Text::CENTERED},
		{"Pol Roca", glm::ivec2(SCREEN_WIDTH / 2, 100), 16, Text::CENTERED},
		{"Adria Lozano", glm::ivec2(SCREEN_WIDTH / 2, 125), 16, Text::CENTERED},
		{"ORIGINAL BY", glm::ivec2(SCREEN_WIDTH / 2, 200), 16, Text::CENTERED},
		{"Juan J. Martinez", glm::ivec2(SCREEN_WIDTH / 2, 225), 16, Text::CENTERED},
		{"ART & MUSIC FROM", glm::ivec2(SCREEN_WIDTH / 2, 275), 16, Text::CENTERED},
		{"Castlevania", glm::ivec2(SCREEN_WIDTH / 2, 300), 16, Text::CENTERED},
		{"freesound.org", glm::ivec2(SCREEN_WIDTH / 2, 325), 16, Text::CENTERED},
	};

	texts[HOW_TO_PLAY1] =
	{
		{"OBJECTIVE", glm::ivec2(SCREEN_WIDTH / 2, SCREEN_HEIGHT/2 - 150), 16, Text::CENTERED},
		{"------------", glm::ivec2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 130), 16, Text::CENTERED},
		{"To beat the game you must step on every platform!", glm::ivec2(SCREEN_WIDTH / 2 - 250, SCREEN_HEIGHT / 2 - 80), 8, Text::LEFT_ALIGNED},
		{"When every platform has been marked, a key will spawn!", glm::ivec2(SCREEN_WIDTH / 2 - 250, SCREEN_HEIGHT / 2 - 60), 8, Text::LEFT_ALIGNED},
		{"Grab the key and head out of the stage as fast as you can!", glm::ivec2(SCREEN_WIDTH / 2 - 250, SCREEN_HEIGHT / 2 - 40), 8, Text::LEFT_ALIGNED},
		{"Marking platforms and completing the level gives you points", glm::ivec2(SCREEN_WIDTH / 2 - 250, SCREEN_HEIGHT / 2 - 20), 8, Text::LEFT_ALIGNED},
		{"Collecting 5000 points will give you an extra life", glm::ivec2(SCREEN_WIDTH / 2 - 250, SCREEN_HEIGHT / 2 + 1), 8, Text::LEFT_ALIGNED},
		{">", glm::ivec2(SCREEN_WIDTH - 40, SCREEN_HEIGHT - 20), 40, Text::CENTERED},
	};
	texts[HOW_TO_PLAY2] =
	{
		{"CONTROLS", glm::ivec2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 150), 16, Text::CENTERED},
		{"------------", glm::ivec2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 130), 16, Text::CENTERED},
		{"ESC: Return to the menu", glm::ivec2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 80), 8, Text::CENTERED},
		{"P: Pause the game", glm::ivec2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 60), 8, Text::CENTERED},
		{"USE THE KEYBOARD ARROWS TO JUMP, CROUCH AND MOVE", glm::ivec2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 20), 8, Text::CENTERED},
		{">", glm::ivec2((SCREEN_WIDTH / 2) + 40, SCREEN_HEIGHT / 2 + 60), 32, Text::CENTERED},
		{"<", glm::ivec2((SCREEN_WIDTH / 2) - 40, SCREEN_HEIGHT / 2 + 60), 32, Text::CENTERED},
		{"^", glm::ivec2((SCREEN_WIDTH / 2) + 0, SCREEN_HEIGHT / 2 -  20 + 60), 32, Text::CENTERED},
		{"v", glm::ivec2((SCREEN_WIDTH / 2) + 0, SCREEN_HEIGHT / 2 - 0 + 60), 32, Text::CENTERED},
		{"<", glm::ivec2(40, SCREEN_HEIGHT - 20), 40, Text::CENTERED},
		{">", glm::ivec2(SCREEN_WIDTH - 40, SCREEN_HEIGHT - 20), 40, Text::CENTERED},

	};

	bghowtoplay = Sprite::createSprite(glm::ivec2(0, 0), glm::vec2(float(SCREEN_WIDTH), float(SCREEN_HEIGHT)), glm::vec2(1.f, 1.f), &bghowtoplaysheet, &texProgram);
	images[HOW_TO_PLAY3] = { bghowtoplay };
	spawnKey(HOW_TO_PLAY3, glm::vec2(SCREEN_WIDTH * 1.25 / 6, 140));
	spawnItems(HOW_TO_PLAY3, glm::vec2(SCREEN_WIDTH * 1.25 / 6, 200), glm::vec2(SCREEN_WIDTH * 1.25 / 6, 260), glm::vec2(SCREEN_WIDTH * 1.25 / 6, 320));
	int subTextSize = 8;

	texts[HOW_TO_PLAY3] =
	{
		{"ITEMS", glm::ivec2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 150), 16, Text::CENTERED},
		{"------------", glm::ivec2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 130), 16, Text::CENTERED},
		{"THERE ARE SOME ITEMS THAT CAN HELP YOU OUT!", glm::ivec2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 80), 10, Text::CENTERED},
		{"Opens the door", glm::ivec2(SCREEN_WIDTH * 1.25 / 6 + 48,								images[HOW_TO_PLAY3][1]->getPosition().y + 16 + subTextSize/2), subTextSize, Text::LEFT_ALIGNED},
		{"Freezes the enemies for 5 seconds", glm::ivec2(SCREEN_WIDTH * 1.25 / 6 + 48,			images[HOW_TO_PLAY3][2]->getPosition().y + 16 + subTextSize/2), subTextSize, Text::LEFT_ALIGNED},
		{"Gives you extra time to complete the level", glm::ivec2(SCREEN_WIDTH * 1.25 / 6 + 48,	images[HOW_TO_PLAY3][3]->getPosition().y + 16 + subTextSize/2), subTextSize, Text::LEFT_ALIGNED},
		{"Gives you extra points", glm::ivec2(SCREEN_WIDTH * 1.25 / 6 + 48,						images[HOW_TO_PLAY3][4]->getPosition().y + 16 + subTextSize/2), subTextSize, Text::LEFT_ALIGNED},
		{"<", glm::ivec2(40, SCREEN_HEIGHT - 20), 40, Text::CENTERED},
		{">", glm::ivec2(SCREEN_WIDTH - 40, SCREEN_HEIGHT - 20), 40, Text::CENTERED},
	};

	images[HOW_TO_PLAY4] = { bghowtoplay };
	spawnEnemies(HOW_TO_PLAY4, glm::vec2(SCREEN_WIDTH * 0.75 / 6, 260), glm::vec2(SCREEN_WIDTH * 2.75 / 6, 260), glm::vec2(SCREEN_WIDTH * 4.25 / 6, 196));

	texts[HOW_TO_PLAY4] =
	{
		{"ENEMIES", glm::ivec2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 150), 16, Text::CENTERED},
		{"------------", glm::ivec2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 130), 16, Text::CENTERED},
		{"WATCH OUT!", glm::ivec2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 80), 10, Text::CENTERED},
		{"THERE ARE SOME CREATURES THAT WANT TO HURT YOU", glm::ivec2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 40), 10, Text::CENTERED},
		{"Walks around the map", glm::ivec2(images[HOW_TO_PLAY4][1]->getPosition().x + 35,200), subTextSize, Text::CENTERED},
		{"Can fly across the map", glm::ivec2(images[HOW_TO_PLAY4][3]->getPosition().x + 20,220), subTextSize, Text::CENTERED},
		{"and go through walls", glm::ivec2(images[HOW_TO_PLAY4][3]->getPosition().x + 20,240), subTextSize, Text::CENTERED},
		{"Can transform into a bat", glm::ivec2(images[HOW_TO_PLAY4][2]->getPosition().x + 64,200), subTextSize, Text::CENTERED},
		{"<", glm::ivec2(40, SCREEN_HEIGHT - 20), 40, Text::CENTERED},
		{">", glm::ivec2(SCREEN_WIDTH - 40, SCREEN_HEIGHT - 20), 40, Text::CENTERED},
	};

	texts[HOW_TO_PLAY5] =
	{
		{"DARK ROOMS", glm::ivec2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 150), 16, Text::CENTERED},
		{"------------", glm::ivec2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 130), 16, Text::CENTERED},
		{"THERE ARE ROOMS IN THE CASTLE WITH NO LIGHT!", glm::ivec2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 80), 10, Text::CENTERED},
		{"You will have to find the door", glm::ivec2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 60), 10, Text::CENTERED},
		{"Use the torches of the walls to see through darkness", glm::ivec2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 40), 10, Text::CENTERED},
		{"DON'T GET LOST!", glm::ivec2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 1), 10, Text::CENTERED},
		{"<", glm::ivec2(40, SCREEN_HEIGHT - 20), 40, Text::CENTERED},
	};

	texts[END] = {
		{"THE END", glm::ivec2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 1), 16, Text::CENTERED},
	};

	bgSpritesheet.loadFromFile("images/titlescreen.png", TEXTURE_PIXEL_FORMAT_RGBA);
	bg = Sprite::createSprite(glm::ivec2(0, 0), glm::vec2(float(SCREEN_WIDTH), float(SCREEN_HEIGHT)), glm::vec2(1.f, 1.f), &bgSpritesheet, &texProgram);
	bgcreditsheet.loadFromFile("images/bgcredits.png", TEXTURE_PIXEL_FORMAT_RGBA);
	bgCredit = Sprite::createSprite(glm::ivec2(0, 0), glm::vec2(float(SCREEN_WIDTH), float(SCREEN_HEIGHT)), glm::vec2(1.f, 1.f), &bgcreditsheet, &texProgram);
	bghowtoplaysheet.loadFromFile("images/bghowtoplay2.png", TEXTURE_PIXEL_FORMAT_RGBA);
	doorSpritesheet.loadFromFile("images/door2.png", TEXTURE_PIXEL_FORMAT_RGBA);
	itemsSpriteSheet.loadFromFile("images/items.png", TEXTURE_PIXEL_FORMAT_RGBA);
	platformsSpritesheet.loadFromFile("images/tileSet2.png", TEXTURE_PIXEL_FORMAT_RGBA);
	manSpriteSheet.loadFromFile("images/soma-animations.png", TEXTURE_PIXEL_FORMAT_RGBA);
	batSpritesheet.loadFromFile("images/bat.png", TEXTURE_PIXEL_FORMAT_RGBA);
	ghostspritesheet.loadFromFile("images/ghost.png", TEXTURE_PIXEL_FORMAT_RGBA);
	skeletonspritesheet.loadFromFile("images/esquelet.png", TEXTURE_PIXEL_FORMAT_RGBA);
	torchSpriteSheet.loadFromFile("images/torch.png", TEXTURE_PIXEL_FORMAT_RGBA);
	endspritesheet.loadFromFile("images/END.png", TEXTURE_PIXEL_FORMAT_RGBA);
	
	Sprite* end = Sprite::createSprite(glm::ivec2(0, 0), glm::vec2(float(SCREEN_WIDTH), float(SCREEN_HEIGHT)), glm::vec2(1.f, 1.f), &endspritesheet, &texProgram);
	images[END] = { end };
	//kbarrowssheet.loadFromFile("images/kbarrows.png", TEXTURE_PIXEL_FORMAT_RGBA);

	images[MAIN_MENU] = { bg };
	
	
	images[CREDITS] = { bgCredit };
	
	
	images[HOW_TO_PLAY1] = { bghowtoplay };
	spawnDoor(HOW_TO_PLAY1);
	spawnKey(HOW_TO_PLAY1, glm::vec2(SCREEN_WIDTH * 4 / 6, 260));
	spawnPlatforms(HOW_TO_PLAY1);
	spawnMan(HOW_TO_PLAY1, glm::vec2(SCREEN_WIDTH * 1 / 6, 260 - 64));

	images[HOW_TO_PLAY2] = { bghowtoplay };
	spawnManMoving(HOW_TO_PLAY2, glm::vec2((SCREEN_WIDTH * 3 / 6) - 40, 260 - 64));
	//spawnArrows(HOW_TO_PLAY2);


	images[HOW_TO_PLAY5] = { bghowtoplay };
	spawnTorch(HOW_TO_PLAY5, glm::vec2(SCREEN_WIDTH / 2 - 16, SCREEN_HEIGHT / 2 + 50));


	

	engine = SoundManager::instance().getSoundEngine();
	menuNavSrc = engine->addSoundSourceFromFile("sound/menu-nav.mp3");
	menuNavSrc->setDefaultVolume(0.5);
}

//Hacemos que devuelva un int o un bool para indicar que quiere cambiar al primer nivel?
void Menu::update(int deltaTime)
{
	if (scene == MAIN_MENU) {
		if (Game::instance().getSpecialKeyUp(GLUT_KEY_UP)) {
			if (--selected < 0) selected = 3;
			engine->play2D(menuNavSrc);
		}
		else if (Game::instance().getSpecialKeyUp(GLUT_KEY_DOWN)) {
			selected = ++selected % 4;
			engine->play2D(menuNavSrc);
		}

		//seleccionar seleccion
		if (Game::instance().getKeyUp('\r') || Game::instance().getKeyUp(10) || Game::instance().getKeyUp(' ')) {
			switch (selected)
			{
			case 0: //Jugar
				//return algo?
				Game::instance().changeLevel(1);
				break;
			case 1: //How to play?
				scene = HOW_TO_PLAY1;
				break;
			case 2: //Creditos
				scene = CREDITS;
				break;
			case 3: //Salir
				exit(0);
				break;
			}
		}

		if (Game::instance().getKeyUp(27)) {
			exit(0);
		}
	}
	else { //Esc, o otra tecla, para volver
		if (Game::instance().getKeyUp(27)) {
			scene = MAIN_MENU;
		}
		moveBetweenHowToPlay();

	}
	for (Sprite* s : images[scene])
	{
		s->update(deltaTime);
	}

}

void Menu::render()
{
	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	for (int i = 0; i < images[scene].size(); ++i)
	{
		if (i == 0) images[scene][i]->render(glm::vec4(0.5f));
		else images[scene][i]->render();
	}

	for (int i = 0; i < texts[scene].size(); ++i)
	{
		glm::vec4 color(1.f);
		if (i == selected && scene == MAIN_MENU) {
			color = glm::vec4(0.7f, 0.8f, 0.2f, 1.f);
		}
		textObj.render(texts[scene][i].text, texts[scene][i].pos + 1, texts[scene][i].textSize, glm::vec4(0, 0, 0, 1) * 0.8f, texts[scene][i].alignment);
		textObj.render(texts[scene][i].text, texts[scene][i].pos, texts[scene][i].textSize, color, texts[scene][i].alignment);
	}


	if (scene == MAIN_MENU) {
		titleObj.render("Night Knight", glm::ivec2(SCREEN_WIDTH / 2+2, SCREEN_HEIGHT / 2 - 65 + 2), 64, glm::vec4(0, 0, 0, 1)*0.8f, Text::CENTERED);
		titleObj.render("Night Knight", glm::ivec2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 65), 64, glm::vec4(0.7, 0.2, 0.1, 1), Text::CENTERED);

		//Renderizar "flecha". Su posicion depende de la variable "selected"

	}

}

void Menu::theEnd()
{
	scene = END;
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

void Menu::moveBetweenHowToPlay()
{
	switch (scene)
	{
	case HOW_TO_PLAY1:
		if (Game::instance().getSpecialKeyUp(GLUT_KEY_RIGHT)) scene = HOW_TO_PLAY2;
		break;
	case HOW_TO_PLAY2:
		if (Game::instance().getSpecialKeyUp(GLUT_KEY_RIGHT)) scene = HOW_TO_PLAY3;
		if (Game::instance().getSpecialKeyUp(GLUT_KEY_LEFT)) scene = HOW_TO_PLAY1;
		break;
	case HOW_TO_PLAY3:
		if (Game::instance().getSpecialKeyUp(GLUT_KEY_RIGHT)) scene = HOW_TO_PLAY4;
		if (Game::instance().getSpecialKeyUp(GLUT_KEY_LEFT)) scene = HOW_TO_PLAY2;
		break;
	case HOW_TO_PLAY4:
		if (Game::instance().getSpecialKeyUp(GLUT_KEY_LEFT)) scene = HOW_TO_PLAY3;
		if (Game::instance().getSpecialKeyUp(GLUT_KEY_RIGHT)) scene = HOW_TO_PLAY5;
		break;
	case HOW_TO_PLAY5:
		if (Game::instance().getSpecialKeyUp(GLUT_KEY_LEFT)) scene = HOW_TO_PLAY4;
		break;
	}
}

void Menu::spawnEnemies(int scene, const glm::vec2& posE, const glm::vec2& posF, const glm::vec2& posV) {
	Sprite* esquelet = Sprite::createSprite(glm::ivec2(0, 0), glm::vec2(64, 64), glm::vec2(1 / 6.f, 1 / 2.f), &skeletonspritesheet, &texProgram);
	esquelet->setDisplacement(glm::vec2(0.0f, 0.0f));
	esquelet->setPosition(posE);
	esquelet->setNumberAnimations(1);
	esquelet->setAnimationParams(0, 10, false);
	esquelet->addKeyframe(0, glm::vec2(0.0f, 0.0f));
	esquelet->addKeyframe(0, glm::vec2(1 / 6.f * 1, 0.0f));
	esquelet->addKeyframe(0, glm::vec2(1 / 6.f * 2, 0.0f));
	esquelet->addKeyframe(0, glm::vec2(1 / 6.f * 3, 0.0f));
	esquelet->addKeyframe(0, glm::vec2(1 / 6.f * 4, 0.0f));
	esquelet->addKeyframe(0, glm::vec2(1 / 6.f * 5, 0.0f));
	esquelet->addKeyframe(0, glm::vec2(1 / 6.f * 0, 1 / 2.f * 1));
	esquelet->addKeyframe(0, glm::vec2(1 / 6.f * 1, 1 / 2.f * 1));
	esquelet->addKeyframe(0, glm::vec2(1 / 6.f * 2, 1 / 2.f * 1));
	esquelet->addKeyframe(0, glm::vec2(1 / 6.f * 3, 1 / 2.f * 1));
	esquelet->addKeyframe(0, glm::vec2(1 / 6.f * 4, 1 / 2.f * 1));
	esquelet->addKeyframe(0, glm::vec2(1 / 6.f * 5, 1 / 2.f * 1));
	esquelet->changeAnimation(0);
	images[scene].push_back(esquelet);

	Sprite* bat = Sprite::createSprite(glm::ivec2(0, 0), glm::vec2(128, 128), glm::vec2(1 / 8.f, 1 / 8.f), &batSpritesheet, &texProgram);
	bat->setPosition(posV);
	bat->setNumberAnimations(2);
	bat->setAnimationParams(0, 10, false, 0, 1);
	bat->addKeyframe(0, glm::vec2(0.0f, 0.0f));
	bat->addKeyframe(0, glm::vec2(1 / 8.f * 1, 0.0f));
	bat->addKeyframe(0, glm::vec2(1 / 8.f * 2, 0.0f));
	bat->addKeyframe(0, glm::vec2(1 / 8.f * 2, 0.0f));
	bat->addKeyframe(0, glm::vec2(1 / 8.f * 1, 0.0f));
	bat->addKeyframe(0, glm::vec2(0.0f, 0.0f));
	bat->addKeyframe(0, glm::vec2(1 / 8.f * 1, 0.0f));
	bat->addKeyframe(0, glm::vec2(1 / 8.f * 2, 0.0f));
	bat->addKeyframe(0, glm::vec2(1 / 8.f * 2, 0.0f));
	bat->addKeyframe(0, glm::vec2(1 / 8.f * 1, 0.0f));
	bat->addKeyframe(0, glm::vec2(0.0f, 0.0f));
	bat->addKeyframe(0, glm::vec2(1 / 8.f * 1, 0.0f));
	bat->addKeyframe(0, glm::vec2(1 / 8.f * 2, 0.0f));
	bat->addKeyframe(0, glm::vec2(1 / 8.f * 2, 0.0f));
	bat->addKeyframe(0, glm::vec2(1 / 8.f * 1, 0.0f));
	bat->addKeyframe(0, glm::vec2(0.0f, 0.0f));
	bat->addKeyframe(0, glm::vec2(1 / 8.f * 1, 0.0f));
	bat->addKeyframe(0, glm::vec2(1 / 8.f * 2, 0.0f));
	bat->addKeyframe(0, glm::vec2(1 / 8.f * 2, 0.0f));
	bat->addKeyframe(0, glm::vec2(1 / 8.f * 1, 0.0f));
	bat->addKeyframe(0, glm::vec2(0.0f, 0.0f));
	bat->addKeyframe(0, glm::vec2(1 / 8.f * 1, 0.0f));
	bat->addKeyframe(0, glm::vec2(1 / 8.f * 2, 0.0f));
	bat->addKeyframe(0, glm::vec2(1 / 8.f * 2, 0.0f));
	bat->addKeyframe(0, glm::vec2(1 / 8.f * 1, 0.0f));
	bat->addKeyframe(0, glm::vec2(0.0f, 0.0f));
	bat->addKeyframe(0, glm::vec2(1 / 8.f * 1, 0.0f));
	bat->addKeyframe(0, glm::vec2(1 / 8.f * 2, 0.0f));
	bat->addKeyframe(0, glm::vec2(1 / 8.f * 2, 0.0f));
	bat->addKeyframe(0, glm::vec2(1 / 8.f * 1, 0.0f));

	bat->setAnimationParams(1, 15, false, 3, 0);
	bat->addKeyframe(1, glm::vec2(0.0f, 1 / 8.f * 2));
	bat->addKeyframe(1, glm::vec2(1 / 8.f * 1, 1 / 8.f * 2));
	bat->addKeyframe(1, glm::vec2(1 / 8.f * 2, 1 / 8.f * 2));
	bat->addKeyframe(1, glm::vec2(1 / 8.f * 3, 1 / 8.f * 2));
	bat->addKeyframe(1, glm::vec2(1 / 8.f * 4, 1 / 8.f * 2));
	bat->addKeyframe(1, glm::vec2(1 / 8.f * 5, 1 / 8.f * 2));
	bat->addKeyframe(1, glm::vec2(1 / 8.f * 6, 1 / 8.f * 2));
	bat->addKeyframe(1, glm::vec2(1 / 8.f * 7, 1 / 8.f * 2));
	bat->addKeyframe(1, glm::vec2(0.0f, 1 / 8.f * 3));
	bat->addKeyframe(1, glm::vec2(1 / 8.f * 7, 1 / 8.f * 2));
	bat->addKeyframe(1, glm::vec2(1 / 8.f * 6, 1 / 8.f * 2));
	bat->addKeyframe(1, glm::vec2(1 / 8.f * 5, 1 / 8.f * 2));
	bat->addKeyframe(1, glm::vec2(1 / 8.f * 4, 1 / 8.f * 2));
	bat->addKeyframe(1, glm::vec2(1 / 8.f * 3, 1 / 8.f * 2));
	bat->addKeyframe(1, glm::vec2(1 / 8.f * 2, 1 / 8.f * 2));
	bat->addKeyframe(1, glm::vec2(1 / 8.f * 3, 1 / 8.f * 2));
	bat->addKeyframe(1, glm::vec2(1 / 8.f * 4, 1 / 8.f * 2));
	bat->addKeyframe(1, glm::vec2(1 / 8.f * 5, 1 / 8.f * 2));
	bat->addKeyframe(1, glm::vec2(1 / 8.f * 6, 1 / 8.f * 2));
	bat->addKeyframe(1, glm::vec2(1 / 8.f * 7, 1 / 8.f * 2));
	bat->addKeyframe(1, glm::vec2(0.0f, 1 / 8.f * 3));
	bat->addKeyframe(1, glm::vec2(1 / 8.f * 7, 1 / 8.f * 2));
	bat->addKeyframe(1, glm::vec2(1 / 8.f * 6, 1 / 8.f * 2));
	bat->addKeyframe(1, glm::vec2(1 / 8.f * 5, 1 / 8.f * 2));
	bat->addKeyframe(1, glm::vec2(1 / 8.f * 4, 1 / 8.f * 2));
	bat->addKeyframe(1, glm::vec2(1 / 8.f * 3, 1 / 8.f * 2));
	bat->addKeyframe(1, glm::vec2(1 / 8.f * 2, 1 / 8.f * 2));
	bat->addKeyframe(1, glm::vec2(1 / 8.f * 3, 1 / 8.f * 2));
	bat->addKeyframe(1, glm::vec2(1 / 8.f * 4, 1 / 8.f * 2));
	bat->addKeyframe(1, glm::vec2(1 / 8.f * 5, 1 / 8.f * 2));
	bat->addKeyframe(1, glm::vec2(1 / 8.f * 6, 1 / 8.f * 2));
	bat->addKeyframe(1, glm::vec2(1 / 8.f * 7, 1 / 8.f * 2));
	bat->addKeyframe(1, glm::vec2(0.0f, 1 / 8.f * 3));
	bat->addKeyframe(1, glm::vec2(1 / 8.f * 7, 1 / 8.f * 2));
	bat->addKeyframe(1, glm::vec2(1 / 8.f * 6, 1 / 8.f * 2));
	bat->addKeyframe(1, glm::vec2(1 / 8.f * 5, 1 / 8.f * 2));
	bat->addKeyframe(1, glm::vec2(1 / 8.f * 4, 1 / 8.f * 2));
	bat->addKeyframe(1, glm::vec2(1 / 8.f * 3, 1 / 8.f * 2));
	bat->addKeyframe(1, glm::vec2(1 / 8.f * 2, 1 / 8.f * 2));

	bat->changeAnimation(0);
	images[scene].push_back(bat);

	Sprite* ghost = Sprite::createSprite(glm::ivec2(0, 0), glm::vec2(64, 64), glm::vec2(1 / 8.f, 1 / 2.f), &ghostspritesheet, &texProgram);
	ghost->setDisplacement(glm::vec2(0.0f, 0.0f));
	ghost->setPosition(posF);
	ghost->setNumberAnimations(1);
	ghost->setAnimationParams(0, 10, false);
	ghost->addKeyframe(0, glm::vec2(1 / 8.f * 7, 0.0f));
	ghost->addKeyframe(0, glm::vec2(1 / 8.f * 6, 0.0f));
	ghost->addKeyframe(0, glm::vec2(1 / 8.f * 5, 0.0f));
	ghost->addKeyframe(0, glm::vec2(1 / 8.f * 4, 0.0f));
	ghost->addKeyframe(0, glm::vec2(1 / 8.f * 3, 0.0f));
	ghost->addKeyframe(0, glm::vec2(1 / 8.f * 2, 0.0f));
	ghost->addKeyframe(0, glm::vec2(1 / 8.f * 1, 0.0f));
	ghost->addKeyframe(0, glm::vec2(1 / 8.f * 0, 0.0f));
	ghost->addKeyframe(0, glm::vec2(1 / 8.f * 7, 1 / 2.f * 1));
	ghost->addKeyframe(0, glm::vec2(1 / 8.f * 6, 1 / 2.f * 1));
	ghost->addKeyframe(0, glm::vec2(1 / 8.f * 7, 1 / 2.f * 1));
	ghost->addKeyframe(0, glm::vec2(1 / 8.f * 6, 1 / 2.f * 1));
	ghost->addKeyframe(0, glm::vec2(1 / 8.f * 7, 1 / 2.f * 1));
	ghost->addKeyframe(0, glm::vec2(1 / 8.f * 6, 1 / 2.f * 1));
	ghost->addKeyframe(0, glm::vec2(1 / 8.f * 7, 1 / 2.f * 1));
	ghost->addKeyframe(0, glm::vec2(1 / 8.f * 6, 1 / 2.f * 1));
	ghost->addKeyframe(0, glm::vec2(1 / 8.f * 7, 1 / 2.f * 1));
	ghost->addKeyframe(0, glm::vec2(1 / 8.f * 6, 1 / 2.f * 1));
	ghost->addKeyframe(0, glm::vec2(1 / 8.f * 7, 1 / 2.f * 1));
	ghost->addKeyframe(0, glm::vec2(1 / 8.f * 0, 0.0f));
	ghost->addKeyframe(0, glm::vec2(1 / 8.f * 1, 0.0f));
	ghost->addKeyframe(0, glm::vec2(1 / 8.f * 2, 0.0f));
	ghost->addKeyframe(0, glm::vec2(1 / 8.f * 3, 0.0f));
	ghost->addKeyframe(0, glm::vec2(1 / 8.f * 4, 0.0f));
	ghost->addKeyframe(0, glm::vec2(1 / 8.f * 5, 0.0f));
	ghost->addKeyframe(0, glm::vec2(1 / 8.f * 6, 0.0f));
	ghost->addKeyframe(0, glm::vec2(1 / 8.f * 6, 0.0f));
	ghost->addKeyframe(0, glm::vec2(1 / 8.f * 6, 0.0f));
	ghost->addKeyframe(0, glm::vec2(1 / 8.f * 6, 0.0f));
	ghost->addKeyframe(0, glm::vec2(1 / 8.f * 6, 0.0f));
	ghost->addKeyframe(0, glm::vec2(1 / 8.f * 6, 0.0f));
	ghost->addKeyframe(0, glm::vec2(1 / 8.f * 6, 0.0f));
	ghost->addKeyframe(0, glm::vec2(1 / 8.f * 6, 0.0f));
	ghost->addKeyframe(0, glm::vec2(1 / 8.f * 6, 0.0f));
	ghost->changeAnimation(0);
	ghost->addEffect(EFFECT_SIN_Y, 2000000);
	images[scene].push_back(ghost);





}

void Menu::spawnItems(int scene, const glm::vec2& posH, const glm::vec2& posC, const glm::vec2& posG) {
	Sprite* hourglass = Sprite::createSprite(glm::ivec2(0, 0), glm::vec2(32, 32), glm::vec2(1 / 8.f, 1 / 8.f), &itemsSpriteSheet, &texProgram);
	hourglass->setDisplacement(glm::vec2(1 / 8.f * 5, 0.0f));
	hourglass->setPosition(posH);
	hourglass->addEffect(EFFECT_SIN_Y, 200000, 750);
	images[scene].push_back(hourglass);

	Sprite* clock = Sprite::createSprite(glm::ivec2(0, 0), glm::vec2(32, 32), glm::vec2(1 / 8.f, 1 / 8.f), &itemsSpriteSheet, &texProgram);
	clock->setDisplacement(glm::vec2(1 / 8.f * 0, 1 / 8.f * 3));
	clock->setPosition(posC);
	clock->setNumberAnimations(1);
	clock->setAnimationParams(0, 8, false);
	clock->addKeyframe(0, glm::vec2(1 / 8.f * 0, 1 / 8.f * 3));
	clock->addKeyframe(0, glm::vec2(1 / 8.f * 1, 1 / 8.f * 3));
	clock->addKeyframe(0, glm::vec2(1 / 8.f * 2, 1 / 8.f * 3));
	clock->addKeyframe(0, glm::vec2(1 / 8.f * 3, 1 / 8.f * 3));
	clock->addKeyframe(0, glm::vec2(1 / 8.f * 4, 1 / 8.f * 3));
	clock->addKeyframe(0, glm::vec2(1 / 8.f * 5, 1 / 8.f * 3));
	clock->addKeyframe(0, glm::vec2(1 / 8.f * 6, 1 / 8.f * 3));
	clock->addKeyframe(0, glm::vec2(1 / 8.f * 7, 1 / 8.f * 3));
	clock->changeAnimation(0);
	clock->addEffect(EFFECT_SIN_Y, 200000, 500);
	images[scene].push_back(clock);

	Sprite* gem = Sprite::createSprite(glm::ivec2(0, 0), glm::vec2(32, 32), glm::vec2(1 / 8.f, 1 / 8.f), &itemsSpriteSheet, &texProgram);
	gem->setDisplacement(glm::vec2(1 / 8.f * 3, 1 / 8.f * 2));
	gem->setPosition(posG);
	gem->addEffect(EFFECT_SIN_Y, 200000, 250);
	images[scene].push_back(gem);

}



void Menu::spawnDoor(int scene) {
	Sprite *door = Sprite::createSprite(glm::ivec2(0, 0), glm::vec2(64, 64), glm::vec2(1 / 4.f, 1 / 4.f), &doorSpritesheet, &texProgram);
	door->setPosition(glm::vec2((SCREEN_WIDTH * 5 / 6) - 32 , 250));
	images[scene].push_back(door);
}

void Menu::spawnKey(int scene, const glm::vec2& pos) {
	Sprite* key = Sprite::createSprite(glm::ivec2(0, 0), glm::vec2(32, 32), glm::vec2(1 / 8.f, 1 / 8.f), &itemsSpriteSheet, &texProgram);
	key->setDisplacement(glm::vec2(1/8.f * 2, 1/8.f * 1));
	key->setPosition(pos);
	key->addEffect(EFFECT_SIN_Y, 200000);
	images[scene].push_back(key);
}

void Menu::spawnPlatforms(int scene) {
	Sprite* plat1 = Sprite::createSprite(glm::ivec2(0, 0), glm::vec2(32, 32), glm::vec2(1 / 16.f, 1 / 16.f), &platformsSpritesheet, &texProgram);
	plat1->setDisplacement(glm::vec2(1 / 16.f * 1, 1 / 16.f * 0));
	plat1->setPosition(glm::vec2((SCREEN_WIDTH * 2 / 6), 260));
	plat1->addEffect(EFFECT_SIN_Y, 200000);
	images[scene].push_back(plat1);

	Sprite* plat2 = Sprite::createSprite(glm::ivec2(0, 0), glm::vec2(32, 32), glm::vec2(1 / 16.f, 1 / 16.f), &platformsSpritesheet, &texProgram);
	plat2->setDisplacement(glm::vec2(1 / 16.f * 2, 1 / 16.f * 0));
	plat2->setPosition(glm::vec2((SCREEN_WIDTH * 2 / 6) + 32, 260));
	plat2->addEffect(EFFECT_SIN_Y, 200000);
	images[scene].push_back(plat2);

	Sprite* plat3 = Sprite::createSprite(glm::ivec2(0, 0), glm::vec2(32, 32), glm::vec2(1 / 16.f, 1 / 16.f), &platformsSpritesheet, &texProgram);
	plat3->setDisplacement(glm::vec2(1 / 16.f * 1, 1 / 16.f * 1));
	plat3->setPosition(glm::vec2(SCREEN_WIDTH * 3 / 6, 260));
	plat3->addEffect(EFFECT_SIN_Y, 200000);
	images[scene].push_back(plat3);

	Sprite* plat4 = Sprite::createSprite(glm::ivec2(0, 0), glm::vec2(32, 32), glm::vec2(1 / 16.f, 1 / 16.f), &platformsSpritesheet, &texProgram);
	plat4->setDisplacement(glm::vec2(1 / 16.f * 2, 1 / 16.f * 1));
	plat4->setPosition(glm::vec2((SCREEN_WIDTH * 3 / 6) + 32, 260));
	plat4->addEffect(EFFECT_SIN_Y, 200000);
	images[scene].push_back(plat4);
}

void Menu::spawnMan(int scene, const glm::vec2& pos) {
	Sprite* man = Sprite::createSprite(glm::ivec2(0, 0), glm::vec2(64, 128), glm::vec2(1 / 16.f, 1 / 16.f), &manSpriteSheet, &texProgram);
	man->setDisplacement(glm::vec2(1 / 16.f * 1, 1 / 16.f * 2));
	man->setPosition(pos);

	man->setNumberAnimations(1);
	man->setAnimationParams(0, 4, false);
	man->addKeyframe(0, glm::vec2(0.0f, 0.0f));
	man->addKeyframe(0, glm::vec2(1 / 16.f * 1, 0.0f));
	man->addKeyframe(0, glm::vec2(1 / 16.f * 2, 0.0f));
	man->addKeyframe(0, glm::vec2(1 / 16.f * 3, 0.0f));
	man->changeAnimation(0);
	images[scene].push_back(man);
}

//void Menu::spawnArrows(int scene) {
//	Sprite* arrows = Sprite::createSprite(glm::ivec2(0, 0), glm::vec2(64, 48), glm::vec2(1 / 1.f, 1 / 1.f), &kbarrowssheet, &texProgram);
//	arrows->setPosition(glm::vec2((SCREEN_WIDTH * 3 / 6) - 20, 150));
//	images[scene].push_back(arrows);
//}


void Menu::spawnManMoving(int scene, const glm::vec2& pos) {
	Sprite* man = Sprite::createSprite(glm::ivec2(0, 0), glm::vec2(64, 128), glm::vec2(1 / 16.f, 1 / 16.f), &manSpriteSheet, &texProgram);
	man->setDisplacement(glm::vec2(1 / 16.f * 1, 1 / 16.f * 2));
	man->setPosition(pos);

	man->setNumberAnimations(2);
	man->setAnimationParams(1, 30, true, 3, 0);
	man->addKeyframe(1, glm::vec2(1 / 16.f * 4, 0.0f));
	man->addKeyframe(1, glm::vec2(1 / 16.f * 5, 0.0f));
	man->addKeyframe(1, glm::vec2(1 / 16.f * 6, 0.0f));
	man->addKeyframe(1, glm::vec2(1 / 16.f * 7, 0.0f));
	man->addKeyframe(1, glm::vec2(1 / 16.f * 8, 0.0f));
	man->addKeyframe(1, glm::vec2(1 / 16.f * 9, 0.0f));
	man->addKeyframe(1, glm::vec2(1 / 16.f * 10, 0.0f));
	man->addKeyframe(1, glm::vec2(1 / 16.f * 11, 0.0f));
	man->addKeyframe(1, glm::vec2(1 / 16.f * 12, 0.0f));
	man->addKeyframe(1, glm::vec2(1 / 16.f * 13, 0.0f));
	man->addKeyframe(1, glm::vec2(1 / 16.f * 14, 0.0f));
	man->addKeyframe(1, glm::vec2(1 / 16.f * 15, 0.0f));
	man->addKeyframe(1, glm::vec2(1 / 16.f * 0, 1 / 16.f * 1));
	man->addKeyframe(1, glm::vec2(1 / 16.f * 1, 1 / 16.f * 1));
	man->addKeyframe(1, glm::vec2(1 / 16.f * 2, 1 / 16.f * 1));
	man->addKeyframe(1, glm::vec2(1 / 16.f * 3, 1 / 16.f * 1));
	man->addKeyframe(1, glm::vec2(1 / 16.f * 4, 1 / 16.f * 1));
	man->addKeyframe(1, glm::vec2(1 / 16.f * 5, 1 / 16.f * 1));
	man->addKeyframe(1, glm::vec2(1 / 16.f * 6, 1 / 16.f * 1));
	man->addKeyframe(1, glm::vec2(1 / 16.f * 7, 0.0f));
	man->addKeyframe(1, glm::vec2(1 / 16.f * 8, 0.0f));
	man->addKeyframe(1, glm::vec2(1 / 16.f * 9, 0.0f));
	man->addKeyframe(1, glm::vec2(1 / 16.f * 10, 0.0f));
	man->addKeyframe(1, glm::vec2(1 / 16.f * 11, 0.0f));
	man->addKeyframe(1, glm::vec2(1 / 16.f * 12, 0.0f));
	man->addKeyframe(1, glm::vec2(1 / 16.f * 13, 0.0f));
	man->addKeyframe(1, glm::vec2(1 / 16.f * 14, 0.0f));
	man->addKeyframe(1, glm::vec2(1 / 16.f * 15, 0.0f));
	man->addKeyframe(1, glm::vec2(1 / 16.f * 0, 1 / 16.f * 1));
	man->addKeyframe(1, glm::vec2(1 / 16.f * 1, 1 / 16.f * 1));
	man->addKeyframe(1, glm::vec2(1 / 16.f * 2, 1 / 16.f * 1));
	man->addKeyframe(1, glm::vec2(1 / 16.f * 3, 1 / 16.f * 1));
	man->addKeyframe(1, glm::vec2(1 / 16.f * 4, 1 / 16.f * 1));
	man->addKeyframe(1, glm::vec2(1 / 16.f * 5, 1 / 16.f * 1));
	man->addKeyframe(1, glm::vec2(1 / 16.f * 6, 1 / 16.f * 1));

	man->setAnimationParams(0, 30, false, 3, 1);
	man->addKeyframe(0, glm::vec2(1 / 16.f * 4, 0.0f));
	man->addKeyframe(0, glm::vec2(1 / 16.f * 5, 0.0f));
	man->addKeyframe(0, glm::vec2(1 / 16.f * 6, 0.0f));
	man->addKeyframe(0, glm::vec2(1 / 16.f * 7, 0.0f));
	man->addKeyframe(0, glm::vec2(1 / 16.f * 8, 0.0f));
	man->addKeyframe(0, glm::vec2(1 / 16.f * 9, 0.0f));
	man->addKeyframe(0, glm::vec2(1 / 16.f * 10, 0.0f));
	man->addKeyframe(0, glm::vec2(1 / 16.f * 11, 0.0f));
	man->addKeyframe(0, glm::vec2(1 / 16.f * 12, 0.0f));
	man->addKeyframe(0, glm::vec2(1 / 16.f * 13, 0.0f));
	man->addKeyframe(0, glm::vec2(1 / 16.f * 14, 0.0f));
	man->addKeyframe(0, glm::vec2(1 / 16.f * 15, 0.0f));
	man->addKeyframe(0, glm::vec2(1 / 16.f * 0, 1 / 16.f * 1));
	man->addKeyframe(0, glm::vec2(1 / 16.f * 1, 1 / 16.f * 1));
	man->addKeyframe(0, glm::vec2(1 / 16.f * 2, 1 / 16.f * 1));
	man->addKeyframe(0, glm::vec2(1 / 16.f * 3, 1 / 16.f * 1));
	man->addKeyframe(0, glm::vec2(1 / 16.f * 4, 1 / 16.f * 1));
	man->addKeyframe(0, glm::vec2(1 / 16.f * 5, 1 / 16.f * 1));
	man->addKeyframe(0, glm::vec2(1 / 16.f * 6, 1 / 16.f * 1));
	man->addKeyframe(0, glm::vec2(1 / 16.f * 7, 0.0f));
	man->addKeyframe(0, glm::vec2(1 / 16.f * 8, 0.0f));
	man->addKeyframe(0, glm::vec2(1 / 16.f * 9, 0.0f));
	man->addKeyframe(0, glm::vec2(1 / 16.f * 10, 0.0f));
	man->addKeyframe(0, glm::vec2(1 / 16.f * 11, 0.0f));
	man->addKeyframe(0, glm::vec2(1 / 16.f * 12, 0.0f));
	man->addKeyframe(0, glm::vec2(1 / 16.f * 13, 0.0f));
	man->addKeyframe(0, glm::vec2(1 / 16.f * 14, 0.0f));
	man->addKeyframe(0, glm::vec2(1 / 16.f * 15, 0.0f));
	man->addKeyframe(0, glm::vec2(1 / 16.f * 0, 1 / 16.f * 1));
	man->addKeyframe(0, glm::vec2(1 / 16.f * 1, 1 / 16.f * 1));
	man->addKeyframe(0, glm::vec2(1 / 16.f * 2, 1 / 16.f * 1));
	man->addKeyframe(0, glm::vec2(1 / 16.f * 3, 1 / 16.f * 1));
	man->addKeyframe(0, glm::vec2(1 / 16.f * 4, 1 / 16.f * 1));
	man->addKeyframe(0, glm::vec2(1 / 16.f * 5, 1 / 16.f * 1));
	man->addKeyframe(0, glm::vec2(1 / 16.f * 6, 1 / 16.f * 1));
	
	
	man->changeAnimation(0);
	images[scene].push_back(man);
}

void Menu::spawnTorch(int scene, const glm::vec2& posT) {
	Sprite* light = Sprite::createSprite(glm::ivec2(0, 0), glm::vec2(32, 64), glm::vec2(1 / 4.f, 1 / 4.f), &torchSpriteSheet, &texProgram);
	light->setPosition(posT);
	light->setNumberAnimations(1);
	light->setAnimationParams(0, 10, false);
	light->addKeyframe(0, glm::vec2(1 / 4.f * 0, 1 / 4.f * 2));
	light->addKeyframe(0, glm::vec2(1 / 4.f * 1, 1 / 4.f * 2));
	light->addKeyframe(0, glm::vec2(1 / 4.f * 2, 1 / 4.f * 2));
	light->addKeyframe(0, glm::vec2(1 / 4.f * 3, 1 / 4.f * 2));
	light->changeAnimation(0);
	light->addEffect(EFFECT_SIN_Y, 200000);
	images[scene].push_back(light);
}
