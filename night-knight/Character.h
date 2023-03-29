#pragma once

#include "Sprite.h"
#include "TileMap.h"

enum {
	EFFECT_BLINK, EFFECT_SHAKE
};

class Character
{
public:
	virtual void init(const glm::ivec2& tileMapPos, bool rightSight, string spriteFile, const glm::ivec2& colliderSize, const glm::ivec2& colliderOffset, const glm::ivec2& pixelSize, const glm::vec2& texSize, ShaderProgram& shaderProgram);
	virtual void update(int deltaTime) = 0;
	void render();

	void setTileMap(TileMap* tileMap);
	void setPosition(const glm::vec2& pos);
	void setSpeed(int speed);
	glm::ivec2 getPosition() const;
	glm::ivec2 getSize() const;

	void freeze(int milisec, bool tremolar);

	~Character();

protected:

	virtual void loadAnimations() = 0;
	void updateTimers(int deltaTime);

	bool bJumping;
	int moveSpeed, moveSpeedBase;

	int freezeTimer;

	glm::ivec2 tileMapDispl, posCharacter;
	int jumpAngle, startY;
	Texture spritesheet;
	Sprite* sprite;
	TileMap* map;
	bool rightSight;
	glm::ivec2 spriteSize;

	glm::ivec2 colliderSize, colliderOffset;

	int effectTimer, effectId;

};

