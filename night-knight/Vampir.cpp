#include "Vampir.h"

#define MOVE_SPEED 1


/*
Comportamiento misterioso ??

//Igual que el fantasma
glm::ivec2 nextPos;
nextPos.x = posCharacter.x + (rightSight * 2 - 1) * MOVE_SPEED;
nextPos.y = posCharacter.y + (!goesUp * 2 - 1) * MOVE_SPEED;

if (nextPos.x < map->LEFT_WALL || nextPos.x + spriteSize.x > map->RIGHT_WALL) {
rightSight = !rightSight;
nextPos.x = posCharacter.x + (rightSight * 2 - 1) * MOVE_SPEED;
}
else if (goesUp && map->collisionMoveUp(posCharacter, glm::ivec2(1, spriteSize.x), false) ||
!goesUp && map->collisionMoveDown(glm::ivec2(posCharacter.x, posCharacter.y + spriteSize.y - 1), glm::ivec2(1, spriteSize.x), &nextPos.y)) {
goesUp = !goesUp;
nextPos.y = posCharacter.y + (goesUp * 2 - 1) * MOVE_SPEED;
}

posCharacter = nextPos;

*/

void Vampir::update(int deltaTime) {
	sprite->update(deltaTime);
	timer += deltaTime;

	if (timer > 6500 && not wantsToTransform) {
		if (isBat) wantsToTransform = true;
		else isBat = true;
		timer = 0;
	}

	if (isBat) {


		/*
		--------------
		|		|
		|		|
		|_______|
		---------------

		colliders justo fuera del collider para poder detectar que entra "justo" entre las plataformas
		*/

		bool collisionUp = map->collisionMoveUp(glm::ivec2(posCharacter.x, posCharacter.y - 1), glm::ivec2(colliderSize.x, 1), false);
		bool collisionDown = map->collisionMoveDown(glm::ivec2(posCharacter.x, posCharacter.y + spriteSize.y), glm::ivec2(colliderSize.x, 1), nullptr);
		if (collisionUp && collisionDown) {
			goesUp = !goesUp;
			posCharacter.x += (rightSight * 2 - 1) * MOVE_SPEED;
		}
		else {
			//Igual que el fantasma
			glm::ivec2 nextPos;
			nextPos.x = posCharacter.x + (rightSight * 2 - 1) * MOVE_SPEED;
			nextPos.y = posCharacter.y + (!goesUp * 2 - 1) * MOVE_SPEED;
		


			bool collisionUp = map->collisionMoveUp(nextPos, glm::ivec2(colliderSize.x, 1), false);
			bool collisionDown = map->collisionMoveDown(glm::ivec2(nextPos.x, nextPos.y + spriteSize.y - 1), glm::ivec2(colliderSize.x, 1), nullptr);


			if (nextPos.x < map->LEFT_WALL || nextPos.x + spriteSize.x > map->RIGHT_WALL) {
				rightSight = !rightSight;
				nextPos.x = posCharacter.x + (rightSight * 2 - 1) * MOVE_SPEED;
			}
			else if (wantsToTransform && collisionDown) {
				wantsToTransform = false;
				isBat = false;
				timer = 0;
				nextPos.y = posCharacter.y; //No cambiamos la y
			}
			else if (goesUp && collisionUp || !goesUp && collisionDown) {
				goesUp = !goesUp;
				nextPos.y = posCharacter.y + (!goesUp * 2 - 1) * MOVE_SPEED;
			}

			posCharacter = nextPos;
		}
		glm::ivec2 nextPos;
		nextPos.x = posCharacter.x + (rightSight * 2 - 1) * MOVE_SPEED;
		nextPos.y = posCharacter.y;

		if (map->collisionMoveLeft(nextPos, colliderSize, false) || map->collisionMoveRight(nextPos, colliderSize, false)) {
			rightSight = !rightSight;
			nextPos.x = posCharacter.x + (rightSight * 2 - 1) * MOVE_SPEED;
		}

	}
	else {
		// Igual que l'esquelet
		bool sightChange = (map->tevacae(posCharacter, spriteSize, rightSight) || map->collisionMoveLeft(posCharacter, spriteSize, false) || map->collisionMoveRight(posCharacter, spriteSize, false));
		if (sightChange) {
			rightSight = !rightSight;
			sprite->changeAnimation(rightSight);
		}
		posCharacter.x += (rightSight * 2 - 1) * MOVE_SPEED;
	}

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posCharacter.x), float(tileMapDispl.y + posCharacter.y)));
}

void Vampir::loadAnimations() {
	sprite->setNumberAnimations(2);

	sprite->setAnimationSpeed(MOVE_LEFT, 10);
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.0f, 0.0f));


	sprite->setAnimationSpeed(MOVE_RIGHT, 10);
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.25f, 0.0f));

	sprite->changeAnimation(rightSight);
}