#include "Esquelet.h"


enum CharacterAnims
{
	MOVE_LEFT, MOVE_RIGHT, STAND_LEFT, STAND_RIGHT
};

void Esquelet::init(const glm::ivec2& tileMapPos, bool rightSight, const glm::vec2& pos, TileMap* map, ShaderProgram& shaderProgram) {
	Character::init(tileMapPos, rightSight, "images/esquelet.png", glm::ivec2(16, 26), glm::ivec2(8, 6), glm::ivec2(32, 32), glm::vec2(1 / 6.f, 1 / 2.f), shaderProgram);
	setPosition(glm::vec2(pos.x * map->getTileSize(), pos.y * map->getTileSize()));
	setTileMap(map);
}


void Esquelet::update(int deltaTime) {
	Character::update(deltaTime);
	bool sightChange = (map->tevacae(posCharacter, spriteSize, rightSight) || map->collisionMoveLeft(posCharacter, colliderOffset, colliderSize, false) || map->collisionMoveRight(posCharacter, colliderOffset, colliderSize, false));
	if (sightChange) {
		rightSight = !rightSight;
		sprite->changeAnimation(rightSight);
	}
	posCharacter.x += (rightSight * 2 - 1) * moveSpeed;
	
	sprite->setPosition(glm::vec2(float(posCharacter.x), float(posCharacter.y)));
}

void Esquelet::loadAnimations() {
	sprite->setNumberAnimations(2);

	sprite->setAnimationParams(MOVE_LEFT, 10, true);
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.0f, 0.0f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(1 / 6.f * 1, 0.0f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(1 / 6.f * 2, 0.0f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(1 / 6.f * 3, 0.0f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(1 / 6.f * 4, 0.0f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(1 / 6.f * 5, 0.0f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(1 / 6.f * 0, 1 / 2.f * 1));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(1 / 6.f * 1, 1 / 2.f * 1));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(1 / 6.f * 2, 1 / 2.f * 1));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(1 / 6.f * 3, 1 / 2.f * 1));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(1 / 6.f * 4, 1 / 2.f * 1));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(1 / 6.f * 5, 1 / 2.f * 1));



	sprite->setAnimationParams(MOVE_RIGHT, 10, false);
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.0f, 0.0f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(1 / 6.f * 1, 0.0f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(1 / 6.f * 2, 0.0f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(1 / 6.f * 3, 0.0f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(1 / 6.f * 4, 0.0f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(1 / 6.f * 5, 0.0f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(1 / 6.f * 0, 1 / 2.f * 1));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(1 / 6.f * 1, 1 / 2.f * 1));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(1 / 6.f * 2, 1 / 2.f * 1));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(1 / 6.f * 3, 1 / 2.f * 1));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(1 / 6.f * 4, 1 / 2.f * 1));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(1 / 6.f * 5, 1 / 2.f * 1));

	sprite->changeAnimation(rightSight);
}