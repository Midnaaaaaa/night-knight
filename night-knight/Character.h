#pragma once

#include "Sprite.h"
#include "TileMap.h"
#include "SoundManager.h"

class Character
{
public:
	virtual void init(const glm::ivec2& tileMapPos, bool rightSight, string spriteFile, const glm::ivec2& colliderSize, const glm::ivec2& colliderOffset, const glm::ivec2& pixelSize, const glm::vec2& texSize, ShaderProgram& shaderProgram);
	virtual void update(int deltaTime);
	virtual void render();

	void setTileMap(TileMap* tileMap);
	void setPosition(const glm::vec2& pos);
	void setSpeed(int speed);
	glm::ivec2 getPosition() const;
	glm::ivec2 getSpriteSize() const;
	glm::ivec2 getColliderPos() const;
	glm::ivec2 getColliderSize() const;
	glm::ivec2 getCenterPos() const;

	void freeze(int milisec, bool tremolar);
	void addEffect(int id, int duration, int delay = 0);
	void addEffect(int id, int duration, const glm::ivec2& point, int delay = 0);


	~Character();

protected:

	virtual void loadAnimations() = 0;
	virtual void updateTimers(int deltaTime, bool freeze = false);

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

	ISoundEngine* engine;


	//int effectDelay, effectTimer, effectId;

};

