#pragma once
#include "Enemy.h"
class Fantasma : public Enemy
{
public:
	virtual void init(const glm::ivec2& tileMapPos, bool rightSight, ShaderProgram& shaderProgram);
	virtual void update(int deltaTime);
	virtual void loadAnimations();
private:
	bool goesUp = true;
};

