#ifndef _PLAYER_INCLUDE
#define _PLAYER_INCLUDE

#include "Character.h"


// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.


class Player : public Character
{

public:
	virtual void init(const glm::ivec2& tileMapPos, bool rightSight, string spriteFile, const glm::ivec2& colliderSize, const glm::ivec2& colliderOffset, const glm::ivec2& pixelSize, const glm::vec2& texSize, ShaderProgram& shaderProgram);
	virtual void update(int deltaTime);
	//virtual void render();

	void checkCollisionUnder();
	bool isHurted();
	bool checkCollisionWithRect(const glm::ivec2 &leftTop, const glm::ivec2 &rightBottom, int type);
	bool inGodMode() const;
	void setGodMode(bool b);

	bool isGameOver() const;


private:
	virtual void loadAnimations();
	void respawn();
	void muelto();

	//bool damaged = false;
	int damagedTimer;

	bool godMode;

	int vidas;
};


#endif // _PLAYER_INCLUDE


