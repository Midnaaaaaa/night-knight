#pragma once
#include "Enemy.h"
class Vampir : public Enemy
{
public:
	virtual void init(const glm::ivec2& tileMapPos, bool rightSight, string spriteFile, const glm::ivec2& colliderSize, const glm::ivec2& colliderOffset, const glm::ivec2& pixelSize, const glm::vec2& texSize, ShaderProgram& shaderProgram);
	virtual void update(int deltaTime);
private:
	virtual void loadAnimations();
	bool isBat;
	bool wantsToTransform;
	bool goesUp;

	int timer;

};

