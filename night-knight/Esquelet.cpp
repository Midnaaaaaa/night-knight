#include "Esquelet.h"


enum CharacterAnims
{
	MOVE_LEFT, MOVE_RIGHT, STAND_LEFT, STAND_RIGHT
};


void Esquelet::update(int deltaTime) {
	sprite->update(deltaTime);
	bool sightChange = (map->tevacae(posCharacter, spriteSize, rightSight) || map->collisionMoveLeft(posCharacter, spriteSize, false) || map->collisionMoveRight(posCharacter, spriteSize, false));
	if (sightChange) {
		rightSight = !rightSight;
		sprite->changeAnimation(rightSight);
	}
	posCharacter.x += (rightSight * 2 - 1) * moveSpeed;
	
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posCharacter.x), float(tileMapDispl.y + posCharacter.y)));
}

void Esquelet::loadAnimations() {
	sprite->setNumberAnimations(2);

	sprite->setAnimationParams(MOVE_LEFT, 10, true);
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.25f, 0.0f));


	sprite->setAnimationParams(MOVE_RIGHT, 10, false);
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.25f, 0.0f));

	sprite->changeAnimation(rightSight);
}