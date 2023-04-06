#include "Fantasma.h"

enum CharacterAnims
{
	MOVE_LEFT, MOVE_RIGHT, STAND_LEFT, STAND_RIGHT
};

void Fantasma::init(const glm::ivec2& tileMapPos, bool rightSight, const glm::vec2& pos, TileMap* map, ShaderProgram& shaderProgram) {
	Character::init(tileMapPos, rightSight, "images/ghost.png", glm::ivec2(16, 20), glm::ivec2(8, 8), glm::ivec2(32, 32), glm::vec2(1 / 8.f, 1 / 2.f), shaderProgram);
	setPosition(glm::vec2(pos.x * map->getTileSize(), pos.y * map->getTileSize()));
	setTileMap(map);
	addEffect(EFFECT_SIN_Y, 200 * 1000);
}


void Fantasma::update(int deltaTime) {
	Character::update(deltaTime);

	//Igual que el fantasma
	glm::ivec2 nextPos;
	nextPos.x = posCharacter.x + (rightSight * 2 - 1) * moveSpeed;
	nextPos.y = posCharacter.y + (!goesUp * 2 - 1) * moveSpeed;

	/*
	--------------
	|		|
	|		|
	|_______|
	---------------

	colliders justo fuera del collider para poder detectar que entra "justo" entre las plataformas
	*/

	bool collisionUp = map->collisionMoveUp(glm::ivec2(posCharacter.x, posCharacter.y - 1), colliderOffset, glm::ivec2(colliderSize.x, 1));
	bool collisionDownFull = map->collisionMoveDown(glm::ivec2(posCharacter.x, posCharacter.y + spriteSize.y), colliderOffset, glm::ivec2(colliderSize.x, 1));
	bool collisionDown = map->collisionMoveDown(glm::ivec2(posCharacter.x, posCharacter.y + colliderOffset.y + colliderSize.y), colliderOffset, glm::ivec2(colliderSize.x, 1));

	if (nextPos.x < map->LEFT_WALL || nextPos.x + spriteSize.x > map->RIGHT_WALL) {
		rightSight = !rightSight;
		nextPos.x = posCharacter.x + (rightSight * 2 - 1) * moveSpeed;
	}
	else if (collisionUp && collisionDownFull) {
		nextPos.y = posCharacter.y;
		goesUp = !goesUp;
	}
	else if (goesUp && collisionUp || !goesUp && collisionDown) {
		goesUp = !goesUp;
		nextPos.y = posCharacter.y + (!goesUp * 2 - 1) * moveSpeed;
	}
	if (rightSight && sprite->animation() != MOVE_RIGHT) sprite->changeAnimation(MOVE_RIGHT);
	else if(!rightSight && sprite->animation() != MOVE_LEFT) sprite->changeAnimation(MOVE_LEFT);

	posCharacter = nextPos;

	sprite->setPosition(glm::vec2(float(posCharacter.x), float(posCharacter.y)));
}

void Fantasma::loadAnimations() {
	sprite->setNumberAnimations(2);

	sprite->setAnimationParams(MOVE_LEFT, 10, true);
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(1 / 8.f * 7, 0.0f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(1 / 8.f * 6, 0.0f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(1 / 8.f * 5, 0.0f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(1 / 8.f * 4, 0.0f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(1 / 8.f * 3, 0.0f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(1 / 8.f * 2, 0.0f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(1 / 8.f * 1, 0.0f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(1 / 8.f * 0, 0.0f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(1 / 8.f * 7, 1 / 2.f * 1));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(1 / 8.f * 6, 1 / 2.f * 1));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(1 / 8.f * 7, 1 / 2.f * 1));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(1 / 8.f * 6, 1 / 2.f * 1));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(1 / 8.f * 7, 1 / 2.f * 1));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(1 / 8.f * 6, 1 / 2.f * 1));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(1 / 8.f * 7, 1 / 2.f * 1));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(1 / 8.f * 6, 1 / 2.f * 1));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(1 / 8.f * 7, 1 / 2.f * 1));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(1 / 8.f * 6, 1 / 2.f * 1));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(1 / 8.f * 7, 1 / 2.f * 1));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(1 / 8.f * 0, 0.0f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(1 / 8.f * 1, 0.0f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(1 / 8.f * 2, 0.0f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(1 / 8.f * 3, 0.0f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(1 / 8.f * 4, 0.0f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(1 / 8.f * 5, 0.0f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(1 / 8.f * 6, 0.0f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(1 / 8.f * 6, 0.0f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(1 / 8.f * 6, 0.0f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(1 / 8.f * 6, 0.0f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(1 / 8.f * 6, 0.0f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(1 / 8.f * 6, 0.0f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(1 / 8.f * 6, 0.0f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(1 / 8.f * 6, 0.0f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(1 / 8.f * 6, 0.0f));










	sprite->setAnimationParams(MOVE_RIGHT, 10, false);
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(1 / 8.f * 7, 0.0f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(1 / 8.f * 6, 0.0f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(1 / 8.f * 5, 0.0f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(1 / 8.f * 4, 0.0f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(1 / 8.f * 3, 0.0f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(1 / 8.f * 2, 0.0f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(1 / 8.f * 1, 0.0f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(1 / 8.f * 0, 0.0f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(1 / 8.f * 7, 1 / 2.f * 1));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(1 / 8.f * 6, 1 / 2.f * 1));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(1 / 8.f * 7, 1 / 2.f * 1));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(1 / 8.f * 6, 1 / 2.f * 1));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(1 / 8.f * 7, 1 / 2.f * 1));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(1 / 8.f * 6, 1 / 2.f * 1));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(1 / 8.f * 7, 1 / 2.f * 1));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(1 / 8.f * 6, 1 / 2.f * 1));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(1 / 8.f * 7, 1 / 2.f * 1));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(1 / 8.f * 6, 1 / 2.f * 1));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(1 / 8.f * 7, 1 / 2.f * 1));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(1 / 8.f * 0, 0.0f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(1 / 8.f * 1, 0.0f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(1 / 8.f * 2, 0.0f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(1 / 8.f * 3, 0.0f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(1 / 8.f * 4, 0.0f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(1 / 8.f * 5, 0.0f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(1 / 8.f * 6, 0.0f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(1 / 8.f * 6, 0.0f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(1 / 8.f * 6, 0.0f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(1 / 8.f * 6, 0.0f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(1 / 8.f * 6, 0.0f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(1 / 8.f * 6, 0.0f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(1 / 8.f * 6, 0.0f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(1 / 8.f * 6, 0.0f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(1 / 8.f * 6, 0.0f));


	sprite->changeAnimation(rightSight);
}
