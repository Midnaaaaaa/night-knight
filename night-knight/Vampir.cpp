#include "Vampir.h"


enum CharacterAnims
{
	MOVE_LEFT, MOVE_RIGHT, FLY_LEFT, FLY_RIGHT
};


void Vampir::init(const glm::ivec2& tileMapPos, bool rightSight, string spriteFile, const glm::ivec2& colliderSize, const glm::ivec2& colliderOffset, const glm::ivec2& pixelSize, const glm::vec2& texSize, ShaderProgram& shaderProgram) {
	Character::init(tileMapPos, rightSight, spriteFile, colliderSize, colliderOffset, pixelSize, texSize, shaderProgram);
	isBat = false;
	wantsToTransform = false;
	goesUp = true;
	timer = 0;
}

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
	Character::update(deltaTime);
	timer += deltaTime;

	if (timer > 6500 && !wantsToTransform) {
		if (isBat) wantsToTransform = true;
		else {
			transformBat();
		}
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

		bool collisionUp = map->collisionMoveUp(glm::ivec2(posCharacter.x, posCharacter.y - 1), colliderOffset, glm::ivec2(colliderSize.x, 1));
		bool collisionDown = map->collisionMoveDown(glm::ivec2(posCharacter.x, posCharacter.y + colliderSize.y), colliderOffset, glm::ivec2(colliderSize.x, 1));
		if (collisionUp && collisionDown) {
			goesUp = !goesUp;
			posCharacter.x += (rightSight * 2 - 1) * moveSpeed;
		}
		else {
			//Igual que el fantasma
			glm::ivec2 nextPos;
			nextPos.x = posCharacter.x + (rightSight * 2 - 1) * moveSpeed;
			nextPos.y = posCharacter.y + (!goesUp * 2 - 1) * moveSpeed;
		


			bool collisionUp = map->collisionMoveUp(nextPos, colliderOffset, glm::ivec2(colliderSize.x, 1));
			bool collisionDown = map->collisionMoveDown(glm::ivec2(nextPos.x, nextPos.y + colliderSize.y - 1), colliderOffset, glm::ivec2(colliderSize.x, 1));


			if (wantsToTransform && collisionDown) {
				transformVampir();
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

		if (map->collisionMoveLeft(nextPos, colliderOffset, colliderSize) || map->collisionMoveRight(nextPos, colliderOffset, colliderSize)) {
			rightSight = !rightSight;
		}

		//Puede ser que en medio del if se haya transformado en man, hay que comprobarlo.
		if (rightSight && isBat && sprite->animation() != FLY_RIGHT) sprite->changeAnimation(FLY_RIGHT);
		else if (!rightSight && isBat && sprite->animation() != FLY_LEFT) sprite->changeAnimation(FLY_LEFT);
	}
	else {
		// Igual que l'esquelet
		bool sightChange = (map->tevacae(posCharacter + colliderOffset, colliderSize, rightSight) || map->collisionMoveLeft(posCharacter, colliderOffset, colliderSize) || map->collisionMoveRight(posCharacter, colliderOffset, colliderSize));
		if (sightChange) {
			rightSight = !rightSight;
			if (rightSight && sprite->animation() != MOVE_RIGHT) sprite->changeAnimation(MOVE_RIGHT);
			else if (!rightSight && sprite->animation() != MOVE_LEFT) sprite->changeAnimation(MOVE_LEFT);
		}
		posCharacter.x += (rightSight * 2 - 1) * moveSpeed;
	}

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posCharacter.x), float(tileMapDispl.y + posCharacter.y)));
}

void Vampir::transformBat() {
	isBat = true;

	colliderSize = glm::ivec2(13, 32);
	colliderOffset = glm::ivec2(26, 24);
}
void Vampir::transformVampir() {
	wantsToTransform = false;
	isBat = false;
	timer = 0;

	posCharacter.y -= spriteSize.y - (colliderOffset.y + colliderSize.y); //Corregir posicion Y

	colliderSize = glm::ivec2(21, 27);
	colliderOffset = glm::ivec2(25, 37);

	if (rightSight && sprite->animation() != MOVE_RIGHT) sprite->changeAnimation(MOVE_RIGHT);
	else if (!rightSight && sprite->animation() != MOVE_LEFT) sprite->changeAnimation(MOVE_LEFT);

}

void Vampir::loadAnimations() {
	sprite->setNumberAnimations(4);

	sprite->setAnimationParams(MOVE_LEFT, 10, true);
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.0f, 0.0f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(1 / 8.f * 1, 0.0f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(1 / 8.f * 2, 0.0f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(1 / 8.f * 2, 0.0f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(1 / 8.f * 1, 0.0f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.0f, 0.0f));


	sprite->setAnimationParams(MOVE_RIGHT, 10, false);
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.0f, 0.0f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(1/8.f * 1, 0.0f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(1 / 8.f * 2, 0.0f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(1 / 8.f * 2, 0.0f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(1 / 8.f * 1, 0.0f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.0f, 0.0f));

	sprite->setAnimationParams(FLY_LEFT, 15, true, 3);
	sprite->addKeyframe(FLY_LEFT, glm::vec2(0.0f, 1 / 8.f * 2));
	sprite->addKeyframe(FLY_LEFT, glm::vec2(1 / 8.f * 1, 1 / 8.f * 2));
	sprite->addKeyframe(FLY_LEFT, glm::vec2(1 / 8.f * 2, 1 / 8.f * 2));
	sprite->addKeyframe(FLY_LEFT, glm::vec2(1 / 8.f * 3, 1 / 8.f * 2));
	sprite->addKeyframe(FLY_LEFT, glm::vec2(1 / 8.f * 4, 1 / 8.f * 2));
	sprite->addKeyframe(FLY_LEFT, glm::vec2(1 / 8.f * 5, 1 / 8.f * 2));
	sprite->addKeyframe(FLY_LEFT, glm::vec2(1 / 8.f * 6, 1 / 8.f * 2));
	sprite->addKeyframe(FLY_LEFT, glm::vec2(1 / 8.f * 7, 1 / 8.f * 2));
	sprite->addKeyframe(FLY_LEFT, glm::vec2(0.0f, 1 / 8.f * 3));
	sprite->addKeyframe(FLY_LEFT, glm::vec2(1 / 8.f * 7, 1 / 8.f * 2));
	sprite->addKeyframe(FLY_LEFT, glm::vec2(1 / 8.f * 6, 1 / 8.f * 2));
	sprite->addKeyframe(FLY_LEFT, glm::vec2(1 / 8.f * 5, 1 / 8.f * 2));
	sprite->addKeyframe(FLY_LEFT, glm::vec2(1 / 8.f * 4, 1 / 8.f * 2));
	sprite->addKeyframe(FLY_LEFT, glm::vec2(1 / 8.f * 3, 1 / 8.f * 2));
	sprite->addKeyframe(FLY_LEFT, glm::vec2(1 / 8.f * 2, 1 / 8.f * 2));


	sprite->setAnimationParams(FLY_RIGHT, 15, false, 3);
	sprite->addKeyframe(FLY_RIGHT, glm::vec2(0.0f, 1 / 8.f * 2));
	sprite->addKeyframe(FLY_RIGHT, glm::vec2(1 / 8.f * 1, 1 / 8.f * 2));
	sprite->addKeyframe(FLY_RIGHT, glm::vec2(1 / 8.f * 2, 1 / 8.f * 2));
	sprite->addKeyframe(FLY_RIGHT, glm::vec2(1 / 8.f * 3, 1 / 8.f * 2));
	sprite->addKeyframe(FLY_RIGHT, glm::vec2(1 / 8.f * 4, 1 / 8.f * 2));
	sprite->addKeyframe(FLY_RIGHT, glm::vec2(1 / 8.f * 5, 1 / 8.f * 2));
	sprite->addKeyframe(FLY_RIGHT, glm::vec2(1 / 8.f * 6, 1 / 8.f * 2));
	sprite->addKeyframe(FLY_RIGHT, glm::vec2(1 / 8.f * 7, 1 / 8.f * 2));
	sprite->addKeyframe(FLY_RIGHT, glm::vec2(0.0f, 1 / 8.f * 3));
	sprite->addKeyframe(FLY_RIGHT, glm::vec2(1 / 8.f * 7, 1 / 8.f * 2));
	sprite->addKeyframe(FLY_RIGHT, glm::vec2(1 / 8.f * 6, 1 / 8.f * 2));
	sprite->addKeyframe(FLY_RIGHT, glm::vec2(1 / 8.f * 5, 1 / 8.f * 2));
	sprite->addKeyframe(FLY_RIGHT, glm::vec2(1 / 8.f * 4, 1 / 8.f * 2));
	sprite->addKeyframe(FLY_RIGHT, glm::vec2(1 / 8.f * 3, 1 / 8.f * 2));
	sprite->addKeyframe(FLY_RIGHT, glm::vec2(1 / 8.f * 2, 1 / 8.f * 2));

	sprite->changeAnimation(rightSight);
}