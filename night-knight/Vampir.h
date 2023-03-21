#pragma once
#include "Enemy.h"
class Vampir : public Enemy
{
	virtual void update(int deltaTime);
	virtual void loadAnimations();
private:
	bool isBat = true;
	bool goesUp = true;

};

