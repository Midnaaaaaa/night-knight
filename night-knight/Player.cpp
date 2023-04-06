#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Player.h"
#include "Game.h"


#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT (96 + 5)/2
#define FALL_STEP 4
//#define PLAYER_OFFSET 8

#define TYPE_ENEMY 1
#define TYPE_KEY 2
#define TYPE_GEM 3
#define TYPE_STOPWATCH 4

#define DAMAGED_TIME 2 * 1000
#define INV_TIME 2 * 1000

#define PUNTS_TO_VIDA_EXTRA 5000

enum CharacterAnims
{
	MOVE_LEFT, MOVE_RIGHT, STAND_LEFT, STAND_RIGHT, CROUCH_LEFT, CROUCH_RIGHT, JUMP_ASCENDING, JUMP_ASCENDING_RIGHT, JUMP_ASCENDING_LEFT, JUMP_ASCENDING_MIRROR, FALLING1, FALLING_MIRROR1, FALLING2, FALLING_MIRROR2, FALLING_NO_JUMP, FALLING_NO_JUMP_MIRROR, FALLING3, FALLING_MIRROR3, TOUCHING_GROUND, TOUCHING_GROUND_MIRROR, MUELTO, MUELTO_MIRROR
};

void Player::init(const glm::ivec2& tileMapPos, bool rightSight, string spriteFile, const glm::ivec2& colliderSize, const glm::ivec2& colliderOffset, const glm::ivec2& pixelSize, const glm::vec2& texSize, const glm::ivec2& spawnPos, ShaderProgram& shaderProgram) {
	Character::init(tileMapPos, rightSight, spriteFile, colliderSize, colliderOffset, pixelSize, texSize, shaderProgram);
	vidas = 3;
	puntuacion = 0;
	this->spawnPos = spawnPos;
	respawn();

	jumpSrc = engine->addSoundSourceFromFile("sound/jump.mp3");
	hitSrc = engine->addSoundSourceFromFile("sound/hit.mp3");
	platformSrc = engine->addSoundSourceFromFile("sound/platform.mp3");
	platformSrc->setDefaultVolume(0.5);
}

Player::~Player() {
	engine->removeSoundSource(jumpSrc);
	engine->removeSoundSource(hitSrc);
	engine->removeSoundSource(platformSrc);

}

void Player::loadAnimations() {
	sprite->setNumberAnimations(22);

	sprite->setAnimationParams(STAND_LEFT, 4, true);
	sprite->addKeyframe(STAND_LEFT, glm::vec2(0.0f, 0.0f));
	sprite->addKeyframe(STAND_LEFT, glm::vec2(1 / 16.f * 1, 0.0f));
	sprite->addKeyframe(STAND_LEFT, glm::vec2(1 / 16.f * 2, 0.0f));
	sprite->addKeyframe(STAND_LEFT, glm::vec2(1 / 16.f * 3, 0.0f));

	sprite->setAnimationParams(STAND_RIGHT, 4, false);
	sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.0f, 0.0f));
	sprite->addKeyframe(STAND_RIGHT, glm::vec2(1 / 16.f * 1, 0.0f));
	sprite->addKeyframe(STAND_RIGHT, glm::vec2(1 / 16.f * 2, 0.0f));
	sprite->addKeyframe(STAND_RIGHT, glm::vec2(1 / 16.f * 3, 0.0f));


	sprite->setAnimationParams(MOVE_LEFT, 30, true, 3);
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(1 / 16.f * 4, 0.0f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(1 / 16.f * 5, 0.0f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(1 / 16.f * 6, 0.0f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(1 / 16.f * 7, 0.0f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(1 / 16.f * 8, 0.0f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(1 / 16.f * 9, 0.0f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(1 / 16.f * 10, 0.0f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(1 / 16.f * 11, 0.0f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(1 / 16.f * 12, 0.0f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(1 / 16.f * 13, 0.0f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(1 / 16.f * 14, 0.0f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(1 / 16.f * 15, 0.0f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(1 / 16.f * 0, 1 / 16.f * 1));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(1 / 16.f * 1, 1 / 16.f * 1));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(1 / 16.f * 2, 1 / 16.f * 1));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(1 / 16.f * 3, 1 / 16.f * 1));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(1 / 16.f * 4, 1 / 16.f * 1));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(1 / 16.f * 5, 1 / 16.f * 1));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(1 / 16.f * 6, 1 / 16.f * 1));

	sprite->setAnimationParams(MOVE_RIGHT, 30, false, 3);
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(1 / 16.f * 4, 0.0f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(1 / 16.f * 5, 0.0f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(1 / 16.f * 6, 0.0f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(1 / 16.f * 7, 0.0f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(1 / 16.f * 8, 0.0f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(1 / 16.f * 9, 0.0f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(1 / 16.f * 10, 0.0f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(1 / 16.f * 11, 0.0f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(1 / 16.f * 12, 0.0f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(1 / 16.f * 13, 0.0f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(1 / 16.f * 14, 0.0f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(1 / 16.f * 15, 0.0f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(1 / 16.f * 0, 1 / 16.f * 1));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(1 / 16.f * 1, 1 / 16.f * 1));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(1 / 16.f * 2, 1 / 16.f * 1));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(1 / 16.f * 3, 1 / 16.f * 1));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(1 / 16.f * 4, 1 / 16.f * 1));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(1 / 16.f * 5, 1 / 16.f * 1));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(1 / 16.f * 6, 1 / 16.f * 1));


	sprite->setAnimationParams(JUMP_ASCENDING, 10, false);
	sprite->addKeyframe(JUMP_ASCENDING, glm::vec2(1 / 16.f * 7, 1 / 16.f * 1));
	sprite->addKeyframe(JUMP_ASCENDING, glm::vec2(1 / 16.f * 8, 1 / 16.f * 1));

	sprite->setAnimationParams(JUMP_ASCENDING_MIRROR, 10, true);
	sprite->addKeyframe(JUMP_ASCENDING_MIRROR, glm::vec2(1 / 16.f * 7, 1 / 16.f * 1));
	sprite->addKeyframe(JUMP_ASCENDING_MIRROR, glm::vec2(1 / 16.f * 8, 1 / 16.f * 1));


	sprite->setAnimationParams(JUMP_ASCENDING_LEFT, 1, true);
	sprite->addKeyframe(JUMP_ASCENDING_LEFT, glm::vec2(1 / 16.f * 9,  1 / 16.f * 1));
	sprite->addKeyframe(JUMP_ASCENDING_LEFT, glm::vec2(1 / 16.f * 10, 1 / 16.f * 1));
	sprite->addKeyframe(JUMP_ASCENDING_LEFT, glm::vec2(1 / 16.f * 11, 1 / 16.f * 1));

	sprite->setAnimationParams(JUMP_ASCENDING_RIGHT, 1, false);
	sprite->addKeyframe(JUMP_ASCENDING_RIGHT, glm::vec2(1 / 16.f * 9, 1 / 16.f * 1));
	sprite->addKeyframe(JUMP_ASCENDING_RIGHT, glm::vec2(1 / 16.f * 10, 1 / 16.f * 1));
	sprite->addKeyframe(JUMP_ASCENDING_RIGHT, glm::vec2(1 / 16.f * 11, 1 / 16.f * 1));



	sprite->setAnimationParams(FALLING1, 1, false);
	sprite->addKeyframe(FALLING1, glm::vec2(1 / 16.f * 12, 1 / 16.f * 1));

	sprite->setAnimationParams(FALLING_MIRROR1, 1, true);
	sprite->addKeyframe(FALLING_MIRROR1, glm::vec2(1 / 16.f * 12, 1 / 16.f * 1));


	sprite->setAnimationParams(FALLING2, 1, false);
	sprite->addKeyframe(FALLING2, glm::vec2(1 / 16.f * 13, 1 / 16.f * 1));

	sprite->setAnimationParams(FALLING_MIRROR2, 1, true);
	sprite->addKeyframe(FALLING_MIRROR2, glm::vec2(1 / 16.f * 13, 1 / 16.f * 1));


	sprite->setAnimationParams(FALLING3, 1, false);
	sprite->addKeyframe(FALLING3, glm::vec2(1 / 16.f * 14, 1 / 16.f * 1));

	sprite->setAnimationParams(FALLING_MIRROR3, 1, true);
	sprite->addKeyframe(FALLING_MIRROR3, glm::vec2(1 / 16.f * 14, 1 / 16.f * 1));


	sprite->setAnimationParams(FALLING_NO_JUMP, 10, false, 2);
	sprite->addKeyframe(FALLING_NO_JUMP, glm::vec2(1 / 16.f * 12, 1 / 16.f * 1));
	sprite->addKeyframe(FALLING_NO_JUMP, glm::vec2(1 / 16.f * 13, 1 / 16.f * 1));
	sprite->addKeyframe(FALLING_NO_JUMP, glm::vec2(1 / 16.f * 14, 1 / 16.f * 1));

	sprite->setAnimationParams(FALLING_NO_JUMP_MIRROR, 10, true, 2);
	sprite->addKeyframe(FALLING_NO_JUMP_MIRROR, glm::vec2(1 / 16.f * 12, 1 / 16.f * 1));
	sprite->addKeyframe(FALLING_NO_JUMP_MIRROR, glm::vec2(1 / 16.f * 13, 1 / 16.f * 1));
	sprite->addKeyframe(FALLING_NO_JUMP_MIRROR, glm::vec2(1 / 16.f * 14, 1 / 16.f * 1));


	sprite->setAnimationParams(TOUCHING_GROUND, 20, false, 0, STAND_RIGHT);
	//sprite->addKeyframe(TOUCHING_GROUND, glm::vec2(1 / 16.f * 4, 1 / 16.f * 2));
	//sprite->addKeyframe(TOUCHING_GROUND, glm::vec2(1 / 16.f * 5, 1 / 16.f * 2));
	sprite->addKeyframe(TOUCHING_GROUND, glm::vec2(1 / 16.f * 6, 1 / 16.f * 2));
	sprite->addKeyframe(TOUCHING_GROUND, glm::vec2(1 / 16.f * 7, 1 / 16.f * 2));
	sprite->addKeyframe(TOUCHING_GROUND, glm::vec2(1 / 16.f * 8, 1 / 16.f * 2));
	sprite->addKeyframe(TOUCHING_GROUND, glm::vec2(1 / 16.f * 9, 1 / 16.f * 2));

	sprite->setAnimationParams(TOUCHING_GROUND_MIRROR, 20, true, 0, STAND_LEFT);
	//sprite->addKeyframe(TOUCHING_GROUND_MIRROR, glm::vec2(1 / 16.f * 4, 1 / 16.f * 2));
	//sprite->addKeyframe(TOUCHING_GROUND_MIRROR, glm::vec2(1 / 16.f * 5, 1 / 16.f * 2));
	sprite->addKeyframe(TOUCHING_GROUND_MIRROR, glm::vec2(1 / 16.f * 6, 1 / 16.f * 2));
	sprite->addKeyframe(TOUCHING_GROUND_MIRROR, glm::vec2(1 / 16.f * 7, 1 / 16.f * 2));
	sprite->addKeyframe(TOUCHING_GROUND_MIRROR, glm::vec2(1 / 16.f * 8, 1 / 16.f * 2));
	sprite->addKeyframe(TOUCHING_GROUND_MIRROR, glm::vec2(1 / 16.f * 9, 1 / 16.f * 2));


	sprite->setAnimationParams(CROUCH_LEFT, 8, true, 3);
	sprite->addKeyframe(CROUCH_LEFT, glm::vec2(1 / 16.f * 0, 1 / 16.f * 2));
	sprite->addKeyframe(CROUCH_LEFT, glm::vec2(1 / 16.f * 1, 1 / 16.f * 2));
	sprite->addKeyframe(CROUCH_LEFT, glm::vec2(1 / 16.f * 2, 1 / 16.f * 2));
	sprite->addKeyframe(CROUCH_LEFT, glm::vec2(1 / 16.f * 3, 1 / 16.f * 2));

	sprite->setAnimationParams(CROUCH_RIGHT, 8, false, 3);
	sprite->addKeyframe(CROUCH_RIGHT, glm::vec2(1 / 16.f * 0, 1 / 16.f * 2));
	sprite->addKeyframe(CROUCH_RIGHT, glm::vec2(1 / 16.f * 1, 1 / 16.f * 2));
	sprite->addKeyframe(CROUCH_RIGHT, glm::vec2(1 / 16.f * 2, 1 / 16.f * 2));
	sprite->addKeyframe(CROUCH_RIGHT, glm::vec2(1 / 16.f * 3, 1 / 16.f * 2));

	sprite->setAnimationParams(MUELTO, 16, false, 4);
	sprite->addKeyframe(MUELTO, glm::vec2(1 / 16.f * 10, 1 / 16.f * 2));
	sprite->addKeyframe(MUELTO, glm::vec2(1 / 16.f * 11, 1 / 16.f * 2));
	sprite->addKeyframe(MUELTO, glm::vec2(1 / 16.f * 12, 1 / 16.f * 2));
	sprite->addKeyframe(MUELTO, glm::vec2(1 / 16.f * 13, 1 / 16.f * 2));
	sprite->addKeyframe(MUELTO, glm::vec2(1 / 16.f * 14, 1 / 16.f * 2));

	sprite->setAnimationParams(MUELTO_MIRROR, 16, true, 4);
	sprite->addKeyframe(MUELTO_MIRROR, glm::vec2(1 / 16.f * 10, 1 / 16.f * 2));
	sprite->addKeyframe(MUELTO_MIRROR, glm::vec2(1 / 16.f * 11, 1 / 16.f * 2));
	sprite->addKeyframe(MUELTO_MIRROR, glm::vec2(1 / 16.f * 12, 1 / 16.f * 2));
	sprite->addKeyframe(MUELTO_MIRROR, glm::vec2(1 / 16.f * 13, 1 / 16.f * 2));
	sprite->addKeyframe(MUELTO_MIRROR, glm::vec2(1 / 16.f * 14, 1 / 16.f * 2));

	sprite->changeAnimation(STAND_RIGHT);
}

void Player::respawn() {
	damagedTimer = 0;
	setPosition(glm::vec2(spawnPos.x * map->getTileSize(), spawnPos.y * map->getTileSize()));
	moveSpeed = 2;
	moveSpeedBase = 2;
	if (rightSight) sprite->changeAnimation(STAND_RIGHT);
	else sprite->changeAnimation(STAND_LEFT);
	if (puntuacion > 0) {
		invulnerabilityTimer = INV_TIME;
		addEffect(EFFECT_BLINK, INV_TIME);
	}
}

void Player::dentroDePlataforma() {
	//Para ver si estamos dentro de una plataforma
	map->dentroDePlataforma(posCharacter, colliderOffset, colliderSize);
}

void Player::update(int deltaTime)
{	
	Character::update(deltaTime);

	if (damagedTimer > 0) {
		damagedTimer -= deltaTime;
		if (damagedTimer <= 0 && vidas > 0) {
			respawn();
		}
	}

	if (invulnerabilityTimer > 0) {
		invulnerabilityTimer -= deltaTime;
		if (invulnerabilityTimer <= 0) {
			invulnerabilityTimer = 0;
		}
	}

	bool dentroPlataforma = bJumping && map->dentroDePlataforma(posCharacter, colliderOffset, colliderSize);
	
	if (Game::instance().getSpecialKey(GLUT_KEY_LEFT) && (sprite->animation() != CROUCH_LEFT && sprite->animation() != CROUCH_RIGHT) && sePuedeMover())
	{
		if (sprite->animation() == MOVE_RIGHT || sprite->animation() == STAND_RIGHT || sprite->animation() == STAND_LEFT || 
			sprite->animation() == TOUCHING_GROUND || sprite->animation() == TOUCHING_GROUND_MIRROR) {
			sprite->changeAnimation(MOVE_LEFT);
		}
		rightSight = false;
		posCharacter.x -= moveSpeed;
		int tileCol = map->collisionMoveLeft(posCharacter, colliderOffset, colliderSize, bJumping && (jumpAngle <= 90 || dentroPlataforma));
		if (tileCol != 0)
		{
			posCharacter.x += moveSpeed;
			if (sprite->animation() == MOVE_LEFT) sprite->changeAnimation(STAND_LEFT);
			if (tileCol == TILE_SPIKE) muelto();
		}
	}
	else if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT) && (sprite->animation() != CROUCH_LEFT && sprite->animation() != CROUCH_RIGHT) && sePuedeMover())
	{
		if (sprite->animation() == MOVE_LEFT || sprite->animation() == STAND_RIGHT || sprite->animation() == STAND_LEFT ||
			sprite->animation() == TOUCHING_GROUND || sprite->animation() == TOUCHING_GROUND_MIRROR) {
			sprite->changeAnimation(MOVE_RIGHT);
		}
		rightSight = true;
		posCharacter.x += moveSpeed;
		int tileCol = map->collisionMoveRight(posCharacter, colliderOffset, colliderSize, bJumping && (jumpAngle <= 90 || dentroPlataforma));
		if (tileCol != 0)
		{
			posCharacter.x -= moveSpeed;
			if (sprite->animation() == MOVE_RIGHT) sprite->changeAnimation(STAND_RIGHT);
			if (tileCol == TILE_SPIKE) muelto();
		}
	}

	if(bJumping)
	{
		//Check if next jump frame is safe, before advancing
		int nextJumpAngle = jumpAngle + JUMP_ANGLE_STEP;

		int nextY = int(startY - JUMP_HEIGHT * sin(3.14159f * nextJumpAngle / 180.f));
		int tileCol = map->collisionMoveUp(glm::ivec2(posCharacter.x, nextY), colliderOffset, colliderSize, true);
		if (tileCol == TILE_SPIKE) {
			jumpAngle = (180 - jumpAngle);
			muelto();
		}

		if(jumpAngle == 180)
		{
			bJumping = false;
			posCharacter.y = startY;
			if (sePuedeMover()) {
				if (rightSight) sprite->changeAnimation(FALLING_NO_JUMP, 2);
				else sprite->changeAnimation(FALLING_NO_JUMP_MIRROR, 2);
			}
		}
		else
		{
			//Advance jumpAngle
			nextY = int(startY - JUMP_HEIGHT * sin(3.14159f * nextJumpAngle / 180.f));
			int lastY = int(startY - JUMP_HEIGHT * sin(3.14159f * lastJumpAngle / 180.f));
			jumpAngle += JUMP_ANGLE_STEP;
			if ((tileCol != 0 && tileCol != TILE_SPIKE) || nextY < lastY - 16) {
				posCharacter.y = int(startY - JUMP_HEIGHT * sin(3.14159f * lastJumpAngle / 180.f));
			}
			else {
				posCharacter.y = int(startY - JUMP_HEIGHT * sin(3.14159f * nextJumpAngle / 180.f));
				lastJumpAngle = nextJumpAngle;
			}

			if (jumpAngle > 90) {
				checkCollisionUnder();
				int tileCol = map->collisionMoveDown(posCharacter, colliderOffset, colliderSize, &posCharacter.y);
				bJumping = tileCol == TILE_NOT_SOLID;
			}
		}
		//Cambiar animaciones de salto
		if (jumpAngle < 90 && sePuedeMover()) {
			if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT)) {
				rightSight = true;
				sprite->changeAnimation(JUMP_ASCENDING_RIGHT);
			}
			else if (Game::instance().getSpecialKey(GLUT_KEY_LEFT)) {
				rightSight = false;
				if (sprite->animation() != JUMP_ASCENDING_LEFT) {
					sprite->changeAnimation(JUMP_ASCENDING_LEFT);
				}
			}
			else {
				if (rightSight && sprite->animation() != JUMP_ASCENDING) {
					sprite->changeAnimation(JUMP_ASCENDING);
				}
				else if(!rightSight && sprite->animation() != JUMP_ASCENDING_MIRROR) sprite->changeAnimation(JUMP_ASCENDING_MIRROR);
			}
		}
		else if (jumpAngle < 135 && sePuedeMover()) {
			if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT)) {
				rightSight = true;
				sprite->changeAnimation(FALLING1);
			}
			else if (Game::instance().getSpecialKey(GLUT_KEY_LEFT)) {
				rightSight = false;
				sprite->changeAnimation(FALLING_MIRROR1);
			}
			else {
				if (rightSight) sprite->changeAnimation(FALLING1);
				else sprite->changeAnimation(FALLING_MIRROR1);
			}
		}
		else if (jumpAngle < 150 && sePuedeMover()) {
			if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT)) {
				rightSight = true;
				sprite->changeAnimation(FALLING2);
			}
			else if (Game::instance().getSpecialKey(GLUT_KEY_LEFT)) {
				rightSight = false;
				sprite->changeAnimation(FALLING_MIRROR2);
			}
			else {
				if (rightSight) sprite->changeAnimation(FALLING2);
				else sprite->changeAnimation(FALLING_MIRROR2);
			}
		}
		else if (jumpAngle > 150 && jumpAngle < 180 && sePuedeMover()) {
			if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT)) {
				rightSight = true;
				sprite->changeAnimation(FALLING3);
			}
			else if (Game::instance().getSpecialKey(GLUT_KEY_LEFT)) {
				rightSight = false;
				sprite->changeAnimation(FALLING_MIRROR3);
			}
			else {
				if (rightSight) sprite->changeAnimation(FALLING3);
				else sprite->changeAnimation(FALLING_MIRROR3);
			}
		}
	}
	else
	{
		posCharacter.y += FALL_STEP;
		checkCollisionUnder();

		//Colision con suelo fuera del salto
		if(map->collisionMoveDown(posCharacter, colliderOffset, colliderSize, &posCharacter.y) != 0 && sePuedeMover())
		{
			if (Game::instance().getSpecialKey(GLUT_KEY_UP))
			{
				bJumping = true;
				jumpAngle = 0;
				lastJumpAngle = 0;
				startY = posCharacter.y;
				engine->play2D(jumpSrc);
			}
			else if (Game::instance().getSpecialKey(GLUT_KEY_DOWN)) { // AGACHARSE
				if (rightSight && sprite->animation() != CROUCH_RIGHT) {
					sprite->changeAnimation(CROUCH_RIGHT);
					colliderOffset.y = 42;
					colliderSize.y = 22;
				}
				else if (!rightSight && sprite->animation() != CROUCH_LEFT) {
					sprite->changeAnimation(CROUCH_LEFT);
					colliderOffset.y = 42;
					colliderSize.y = 22;
				}
			}
			else if (Game::instance().getSpecialKey(GLUT_KEY_LEFT)) {
				if ((sprite->animation() >= FALLING1 && sprite->animation() <= FALLING_MIRROR3)) {
					sprite->changeAnimation(MOVE_LEFT, 3);
					rightSight = false;
				}
			}
			else if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT)) {
				if ((sprite->animation() >= FALLING1 && sprite->animation() <= FALLING_MIRROR3)) {
					sprite->changeAnimation(MOVE_RIGHT, 3);
					rightSight = true;
				}
			}
			else //TOCANDO EL SUELO sin tocar ninguna tecla
			{	
				int anim = -1;
				int animMirror = -1;
				int startFrame = 0;

				//Tocando el suelo luego de caer
				if (sprite->animation() == FALLING3 || sprite->animation() == FALLING_MIRROR3 || sprite->animation() == FALLING_NO_JUMP || sprite->animation() == FALLING_NO_JUMP_MIRROR) {
					anim = TOUCHING_GROUND;
					animMirror = TOUCHING_GROUND_MIRROR;
				}
				else if (sprite->animation() == MOVE_LEFT || sprite->animation() == MOVE_RIGHT || sprite->animation() == CROUCH_LEFT || sprite->animation() == CROUCH_RIGHT) {
					anim = STAND_RIGHT;
					animMirror = STAND_LEFT;
				}
				else if (sprite->animation() >= FALLING1 && sprite->animation() <= FALLING_MIRROR2) {
					anim = TOUCHING_GROUND;
					animMirror = TOUCHING_GROUND_MIRROR;
					startFrame = 2;
				}

				if (anim != -1 && rightSight && (sprite->animation() != anim)) sprite->changeAnimation(anim, startFrame);
				else if (animMirror != -1 && !rightSight && (sprite->animation() != animMirror)) sprite->changeAnimation(animMirror, startFrame);
			}
		}
		else if (sePuedeMover()) {
			if (rightSight) {
				if (sprite->animation() != FALLING_NO_JUMP) sprite->changeAnimation(FALLING_NO_JUMP);
			}
			else {
				if (sprite->animation() != FALLING_NO_JUMP_MIRROR) sprite->changeAnimation(FALLING_NO_JUMP_MIRROR);
			}
		}
	}

	if (sprite->animation() != CROUCH_LEFT && sprite->animation() != CROUCH_RIGHT) {
		colliderOffset.y = 32;
		colliderSize.y = 32;
	}

	sprite->setPosition(glm::vec2(float(posCharacter.x), float(posCharacter.y)));
}

void Player::checkCollisionUnder() {
	int tileSize = map->getTileSize();
	for (int i = 0; i < 2; ++i) {
		int offset = (i == 0) ? 0 : colliderSize.x - 1;
		int tileCol = map->collisionMoveDown(glm::ivec2(posCharacter.x + colliderOffset.x + offset, posCharacter.y + colliderOffset.y), glm::ivec2(1, colliderSize.y));
		if (tileCol == TILE_PLATFORM) {
			int tile = map->modifyTileMap((posCharacter.y + colliderOffset.y + colliderSize.y) / tileSize, (posCharacter.x + colliderOffset.x + offset) / tileSize, /*4 * 8 + 5*/ -16);
			if (tile == -1) continue;
			//                                                                                     (tile * -2 - 3) ----> [1 2] -> [+1 -1] | [1 2] *-2 [-2 -4] +3 [1 -1]
			map->modifyTileMap((posCharacter.y + colliderOffset.y + colliderSize.y) / tileSize, ((posCharacter.x + colliderOffset.x + offset) / tileSize) + (tile * -2 + 3), /*4 * 8 + 5*/ -16);
			map->reduceNumberOfPlatforms();
			map->reduceNumberOfPlatforms();
			increasePuntuacion(10);

			engine->play2D(platformSrc);
			
		}

	}


	//los puntos de colision estan mas hacia el centro
	for (int i = 0; i < 2; ++i) {
		int offset = (i == 0) ? 6 : colliderSize.x - 7;
		int tileCol = map->collisionMoveDown(glm::ivec2(posCharacter.x + colliderOffset.x + offset, posCharacter.y + colliderOffset.y), glm::ivec2(1, colliderSize.y));
		if (tileCol == TILE_SPIKE && !isHurted()) { //Pinxo
			muelto();
		}

	}
}

void Player::render() {
	sprite->render();
	if (godMode) sprite->render(glm::vec4(1,1,0.4,1));
}

void Player::muelto() {
	if (godMode) return;
	damagedTimer = DAMAGED_TIME;
	moveSpeed = 0;
	if(rightSight) sprite->changeAnimation(MUELTO);
	else sprite->changeAnimation(MUELTO_MIRROR);
	if (--vidas <= 0) {
		morir();
	}
	else {
		addEffect(EFFECT_BLINK, DAMAGED_TIME);
		engine->play2D(hitSrc);
	}
}

bool Player::sePuedeMover() {
	return !(sprite->animation() == MUELTO || sprite->animation() == MUELTO_MIRROR || moveSpeed == 0);
}

void Player::setVidas(int vidas)
{
	this->vidas = vidas;
}


bool Player::isHurted() {
	return damagedTimer > 0 || vidas <= 0 || invulnerabilityTimer > 0;
}

bool Player::checkCollisionWithRect(const glm::ivec2& leftTop, const glm::ivec2& rightBottom, int type) {
	glm::ivec2 p0(posCharacter.x + colliderOffset.x, posCharacter.y + colliderOffset.y);
	glm::ivec2 p1(posCharacter.x + colliderOffset.x + colliderSize.x, posCharacter.y + colliderOffset.y + colliderSize.y);
	if (p0.x < rightBottom.x && p1.x > leftTop.x && p0.y < rightBottom.y && p1.y > leftTop.y) {
		if (type == TYPE_ENEMY) muelto();
		return true;
	}
	return false;
}

bool Player::inGodMode() const {
	return godMode;
}
void Player::setGodMode(bool b) {
	godMode = b;
}

bool Player::isGameOver() const {
	return vidas <= 0;
}


int Player::getVidas() const{
	return vidas;
}

void Player::increasePuntuacion(int valueToIncrease) {
	if (puntuacion % PUNTS_TO_VIDA_EXTRA > (puntuacion + valueToIncrease) % PUNTS_TO_VIDA_EXTRA) {
		++vidas;
		engine->play2D("sound/1up.mp3");
	}
	puntuacion += valueToIncrease;
}

int Player::getPuntuacion() const {
	return puntuacion;
}

void Player::morir()
{
	engine->play2D("sound/alu_aaah.wav");
	sprite->setAnimationParams(MUELTO, 3, false, 4);
	sprite->setAnimationParams(MUELTO_MIRROR, 3, true, 4);
}
