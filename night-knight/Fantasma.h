#pragma once
#include "Enemy.h"
class Fantasma : public Enemy
{
public:
	virtual void init(const glm::ivec2& tileMapPos, bool rightSight, string spriteFile, const glm::ivec2& colliderSize, const glm::ivec2& colliderOffset, const glm::ivec2& pixelSize, const glm::vec2& texSize, ShaderProgram& shaderProgram);
	virtual void update(int deltaTime);
	virtual void loadAnimations();
private:
	bool goesUp = true;
};

