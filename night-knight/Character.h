#pragma once

#include "Sprite.h"
#include "TileMap.h"

class Character
{
public:
	void init(const glm::ivec2& tileMapPos, string spriteFile, const glm::ivec2& colliderSize, const glm::ivec2& colliderOffset, const glm::ivec2& pixelSize, const glm::vec2& texSize, ShaderProgram& shaderProgram);
	virtual void update(int deltaTime) = 0;
	void render();

	void setTileMap(TileMap* tileMap);
	void setPosition(const glm::vec2& pos);

protected:

	virtual void loadAnimations() = 0;

	bool bJumping;
	glm::ivec2 tileMapDispl, posCharacter;
	int jumpAngle, startY;
	Texture spritesheet;
	Sprite* sprite;
	TileMap* map;
	bool rightSight;

	glm::ivec2 colliderSize, colliderOffset;

};

