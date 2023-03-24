#include "Vampir.h"


enum CharacterAnims
{
	MOVE_LEFT, MOVE_RIGHT, STAND_LEFT, STAND_RIGHT, FLY
};


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

		bool collisionUp = map->collisionMoveUp(glm::ivec2(posCharacter.x, posCharacter.y - 1), glm::ivec2(colliderSize.x, 1));
		bool collisionDown = map->collisionMoveDown(glm::ivec2(posCharacter.x, posCharacter.y + spriteSize.y), glm::ivec2(colliderSize.x, 1));
		if (collisionUp && collisionDown) {
			goesUp = !goesUp;
			posCharacter.x += (rightSight * 2 - 1) * moveSpeed;
		}
		else {
			//Igual que el fantasma
			glm::ivec2 nextPos;
			nextPos.x = posCharacter.x + (rightSight * 2 - 1) * moveSpeed;
			nextPos.y = posCharacter.y + (!goesUp * 2 - 1) * moveSpeed;
		


			bool collisionUp = map->collisionMoveUp(nextPos, glm::ivec2(colliderSize.x, 1));
			bool collisionDown = map->collisionMoveDown(glm::ivec2(nextPos.x, nextPos.y + spriteSize.y - 1), glm::ivec2(colliderSize.x, 1));


			if (wantsToTransform && collisionDown) {
				wantsToTransform = false;
				isBat = false;
				timer = 0;
				nextPos.y = posCharacter.y; //No cambiamos la y
			}
			else if (goesUp && collisionUp || !goesUp && collisionDown) {
				goesUp = !goesUp;
				nextPos.y = posCharacter.y + (!goesUp * 2 - 1) * moveSpeed;
			}

			posCharacter = nextPos;
		}

		//Comprobar la posicion del siguiente frame. Si colisionaria horizontalmente, cambiamos la direccion (pero no la posicion), 
		//asi en el siguiente frame ya está girado y no va a colisionar
		glm::ivec2 nextPos;
		nextPos.x = posCharacter.x + (rightSight * 2 - 1) * moveSpeed;
		nextPos.y = posCharacter.y;

		if (map->collisionMoveLeft(nextPos, colliderSize, false) || map->collisionMoveRight(nextPos, colliderSize)) {
			rightSight = !rightSight;
		}

	}
	else {
		// Igual que l'esquelet
		bool sightChange = (map->tevacae(posCharacter, spriteSize, rightSight) || map->collisionMoveLeft(posCharacter, spriteSize) || map->collisionMoveRight(posCharacter, spriteSize));
		if (sightChange) {
			rightSight = !rightSight;
			sprite->changeAnimation(rightSight);
		}
		posCharacter.x += (rightSight * 2 - 1) * moveSpeed;
	}

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posCharacter.x), float(tileMapDispl.y + posCharacter.y)));
}

void Vampir::loadAnimations() {
	sprite->setNumberAnimations(2);

	sprite->setAnimationParams(MOVE_LEFT, 10);
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.0f, 0.0f));


	sprite->setAnimationParams(MOVE_RIGHT, 10);
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.25f, 0.0f));

	sprite->changeAnimation(rightSight);
}