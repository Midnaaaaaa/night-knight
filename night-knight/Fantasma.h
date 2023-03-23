#pragma once
#include "Enemy.h"
class Fantasma : public Enemy
{
	virtual void update(int deltaTime);
	virtual void loadAnimations();
private:
	bool goesUp = true;
};

