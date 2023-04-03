#pragma once
#include "Enemy.h"
class Vampir : public Enemy
{
public:
	virtual void init(const glm::ivec2& tileMapPos, bool rightSight, ShaderProgram& shaderProgram);
	virtual void update(int deltaTime);
private:
	virtual void loadAnimations();

	virtual void updateTimers(int deltaTime, bool freeze);

	void transformBat();
	void transformVampir();

	bool isBat;
	bool wantsToTransform;
	bool goesUp;

	int timer, transformTimer;

};

