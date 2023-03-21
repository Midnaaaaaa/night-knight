#pragma once
#include "Enemy.h"
class Esquelet : public Enemy
{
    virtual void update(int deltaTime);
    virtual void loadAnimations();
};

