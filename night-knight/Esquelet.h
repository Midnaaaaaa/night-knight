#pragma once
#include "Enemy.h"
class Esquelet : public Enemy
{
public:
    virtual void init(const glm::ivec2& tileMapPos, bool rightSight, const glm::vec2& pos, TileMap* map, ShaderProgram& shaderProgram);
    virtual void update(int deltaTime);
private:
    virtual void loadAnimations();
};

