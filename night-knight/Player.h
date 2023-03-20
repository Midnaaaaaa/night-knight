#ifndef _PLAYER_INCLUDE
#define _PLAYER_INCLUDE

#include "Character.h"


// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.


class Player : public Character
{

public:
	virtual void update(int deltaTime);

	void checkCollisionWithPlatform();

private:
	virtual void loadAnimations();

};


#endif // _PLAYER_INCLUDE


