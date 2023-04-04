#pragma once
#include "Character.h"

class Enemy : public Character {
public:
	virtual void init(const glm::ivec2& tileMapPos, bool rightSight, const glm::vec2& pos, TileMap* map, ShaderProgram& shaderProgram) = 0;
};

