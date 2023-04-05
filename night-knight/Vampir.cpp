#include "Vampir.h"

#define TRANSFORM_TIME 1000

enum CharacterAnims
{
	MOVE_LEFT, MOVE_RIGHT, FLY_LEFT, FLY_RIGHT
};


void Vampir::init(const glm::ivec2& tileMapPos, bool rightSight, const glm::vec2& pos, TileMap *map, ShaderProgram& shaderProgram) {
	Character::init(tileMapPos, rightSight, "images/bat.png", glm::ivec2(14, 24), glm::ivec2(25, 40), glm::ivec2(64, 64), glm::vec2(1 / 8.f, 1 / 8.f), shaderProgram);
	setPosition(glm::vec2((pos.x - 2) * map->getTileSize(), (pos.y - 2) * map->getTileSize()));
	setTileMap(map);

	realColliderOffset = glm::ivec2(25, 40);
	realColliderSize = glm::ivec2(14, 24);

	isBat = false;
	wantsToTransform = false;
	goesUp = true;
	timer = 0;

	nextTransformTime = 6500;
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

	if (timer > nextTransformTime && !wantsToTransform) {
		if (isBat) wantsToTransform = true;
		else {
			//transformBat();
			addEffect(EFFECT_SPAWN, TRANSFORM_TIME);
			transformTimer = TRANSFORM_TIME/2;
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

		bool collisionUp = map->collisionMoveUp(glm::ivec2(posCharacter.x, posCharacter.y - 1), realColliderOffset, glm::ivec2(realColliderSize.x, 1));
		int collisionDown = map->collisionMoveDown(glm::ivec2(posCharacter.x, posCharacter.y + realColliderSize.y), realColliderOffset, glm::ivec2(realColliderSize.x, 1));
		if (collisionUp && collisionDown) {
			goesUp = !goesUp;
			posCharacter.x += (rightSight * 2 - 1) * moveSpeed;
		}
		else {
			//Igual que el fantasma
			glm::ivec2 nextPos;
			nextPos.x = posCharacter.x + (rightSight * 2 - 1) * moveSpeed;
			nextPos.y = posCharacter.y + (!goesUp * 2 - 1) * moveSpeed;
		


			bool collisionUp = map->collisionMoveUp(nextPos, realColliderOffset, glm::ivec2(realColliderSize.x, 1));
			bool collisionDown = map->collisionMoveDown(glm::ivec2(nextPos.x, nextPos.y + realColliderSize.y - 1), realColliderOffset, glm::ivec2(realColliderSize.x, 1));


			if (wantsToTransform && collisionDown && collisionDown != TILE_SPIKE && transformTimer == 0) {
				transformVampir();
				//Empieza la transformacion en el MAN, pero la skin le cambia luego
				addEffect(EFFECT_SPAWN, TRANSFORM_TIME);
				transformTimer = TRANSFORM_TIME/2;
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

		if (map->collisionMoveLeft(nextPos, realColliderOffset, realColliderSize) || map->collisionMoveRight(nextPos, realColliderOffset, realColliderSize)) {
			rightSight = !rightSight;
		}

		//Puede ser que en medio del if se haya transformado en man, hay que comprobarlo.
		if (rightSight && isBat && sprite->animation() != FLY_RIGHT) sprite->changeAnimation(FLY_RIGHT);
		else if (!rightSight && isBat && sprite->animation() != FLY_LEFT) sprite->changeAnimation(FLY_LEFT);
	}
	else {
		// Igual que l'esquelet
		bool sightChange = (map->tevacae(posCharacter + realColliderOffset, realColliderSize, rightSight) || map->collisionMoveLeft(posCharacter, realColliderOffset, realColliderSize) || map->collisionMoveRight(posCharacter, realColliderOffset, realColliderSize));
		if (sightChange) {
			rightSight = !rightSight;
			if (rightSight && sprite->animation() != MOVE_RIGHT) sprite->changeAnimation(MOVE_RIGHT);
			else if (!rightSight && sprite->animation() != MOVE_LEFT) sprite->changeAnimation(MOVE_LEFT);
		}
		posCharacter.x += (rightSight * 2 - 1) * moveSpeed;
	}

	sprite->setPosition(glm::vec2(float(posCharacter.x), float(posCharacter.y)));
}

void Vampir::transformBat() {
	isBat = true;
	nextTransformTime = rand() % 5000 + 3000; //de 3 seg a 8 seg


	realColliderSize = glm::ivec2(13, 32);
	realColliderOffset = glm::ivec2(26, 24);

	colliderSize = glm::ivec2(13,6);
	colliderOffset = glm::ivec2(26,37);

	posCharacter.y += spriteSize.y - (realColliderOffset.y + realColliderSize.y); //Corregir posicion Y
}
void Vampir::transformVampir() {
	wantsToTransform = false;
	isBat = false;
	timer = 0;
	nextTransformTime = rand() % 5000 + 3000; //de 3 seg a 8 seg

	posCharacter.y -= spriteSize.y - (realColliderOffset.y + realColliderSize.y); //Corregir posicion Y
 
	realColliderSize = glm::ivec2(14, 24);
	realColliderOffset = glm::ivec2(25, 40);

	colliderSize = glm::ivec2(14, 24);
	colliderOffset = glm::ivec2(25, 40);

	//Cambio la animacion del MAN cuando se acabe el timer
	//if (rightSight && sprite->animation() != MOVE_RIGHT) sprite->changeAnimation(MOVE_RIGHT);
	//else if (!rightSight && sprite->animation() != MOVE_LEFT) sprite->changeAnimation(MOVE_LEFT);

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

void Vampir::updateTimers(int deltaTime, bool freeze) {
	Character::updateTimers(deltaTime);
	if (freeze) return;
	timer += deltaTime;


	if (transformTimer != 0) {
		transformTimer -= deltaTime;
		if (transformTimer <= 0) {
			if (sprite->animation() == FLY_LEFT || sprite->animation() == FLY_RIGHT) {//transformVampir();
				//Ya es MAN en verdad, pero con skin de Bat
				if (rightSight && sprite->animation() != MOVE_RIGHT) sprite->changeAnimation(MOVE_RIGHT);
				else if (!rightSight && sprite->animation() != MOVE_LEFT) sprite->changeAnimation(MOVE_LEFT);
			}
			else transformBat();
			transformTimer = 0;
		}
	}
}
