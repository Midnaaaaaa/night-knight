#ifndef _GAME_INCLUDE
#define _GAME_INCLUDE


#include "Scene.h"
#include "Menu.h"
#include "SoundManager.h"



#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT (16*2+16*22 + 1)





// Game is a singleton (a class with a single instance) that represents our whole application


class Game
{

private:
	Game() {}
	
public:
	static Game &instance()
	{
		static Game G;
	
		return G;
	}
	
	void init();
	bool update(int deltaTime);
	void render();
	
	// Input callback methods
	void keyPressed(int key);
	void keyReleased(int key);
	void specialKeyPressed(int key);
	void specialKeyReleased(int key);
	void mouseMove(int x, int y);
	void mousePress(int button);
	void mouseRelease(int button);
	
	bool getKey(int key) const;
	bool getKeyUp(int key) const;
	bool getSpecialKey(int key) const;
	bool getSpecialKeyUp(int key) const;

	void toggleMenu();

	void exitLevel();
	void changeLevel(int level);

private:
	bool playing;
	bool bPlay;                       // Continue to play game?
	Scene* scene;                      // Scene to render
	Menu menu;					      // Menu
	bool keys[256], specialKeys[256]; // Store key states so that 
	                                  // we can have access at any time
	bool old_keys[256], old_specialKeys[256];

	ISoundEngine* engine;
	ISound* bgSound;

};


#endif // _GAME_INCLUDE


