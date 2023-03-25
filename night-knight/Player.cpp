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

enum CharacterAnims
{
	MOVE_LEFT, MOVE_RIGHT, STAND_LEFT, STAND_RIGHT, CROUCH_LEFT, CROUCH_RIGHT, JUMP_ASCENDING, JUMP_ASCENDING_RIGHT, JUMP_ASCENDING_LEFT, JUMP_ASCENDING_MIRROR, FALLING1, FALLING_MIRROR1, FALLING2, FALLING_MIRROR2, FALLING3, FALLING_MIRROR3, FALLING_LEFT1, FALLING_RIGHT1, FALLING_LEFT2, FALLING_RIGHT2, FALLING_LEFT3, FALLING_RIGHT3, FALL_ANIMATION, FALL_ANIMATION_RIGHT, FALL_ANIMATION_LEFT, FALL_ANIMATION_MIRROR
};



void Player::loadAnimations() {
	sprite->setNumberAnimations(26);

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


	sprite->setAnimationParams(MOVE_LEFT, 25, true, 3);
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

	sprite->setAnimationParams(MOVE_RIGHT, 25, false, 3);
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




	sprite->setAnimationParams(FALLING_MIRROR1, 1, true);
	sprite->addKeyframe(FALLING_MIRROR1, glm::vec2(1 / 16.f * 9, 1 / 16.f * 1));

	sprite->setAnimationParams(FALLING1, 1, false);
	sprite->addKeyframe(FALLING1, glm::vec2(1 / 16.f * 9, 1 / 16.f * 1));


	sprite->setAnimationParams(FALLING_MIRROR2, 1, true);
	sprite->addKeyframe(FALLING_MIRROR2, glm::vec2(1 / 16.f * 10, 1 / 16.f * 1));

	sprite->setAnimationParams(FALLING2, 1, false);
	sprite->addKeyframe(FALLING2, glm::vec2(1 / 16.f * 10, 1 / 16.f * 1));



	sprite->setAnimationParams(FALLING_MIRROR3, 1, true);
	sprite->addKeyframe(FALLING_MIRROR3, glm::vec2(1 / 16.f * 11, 1 / 16.f * 1));

	sprite->setAnimationParams(FALLING3, 1, false);
	sprite->addKeyframe(FALLING3, glm::vec2(1 / 16.f * 11, 1 / 16.f * 1));


	sprite->setAnimationParams(FALLING_LEFT1, 1, true);
	sprite->addKeyframe(FALLING_LEFT1, glm::vec2(1 / 16.f * 12, 1 / 16.f * 1));

	sprite->setAnimationParams(FALLING_RIGHT1, 1, false);
	sprite->addKeyframe(FALLING_RIGHT1, glm::vec2(1 / 16.f * 12, 1 / 16.f * 1));


	sprite->setAnimationParams(FALLING_LEFT2, 1, true);
	sprite->addKeyframe(FALLING_LEFT2, glm::vec2(1 / 16.f * 13, 1 / 16.f * 1));

	sprite->setAnimationParams(FALLING_RIGHT2, 1, false);
	sprite->addKeyframe(FALLING_RIGHT2, glm::vec2(1 / 16.f * 13, 1 / 16.f * 1));


	sprite->setAnimationParams(FALLING_LEFT3, 1, true);
	sprite->addKeyframe(FALLING_LEFT3, glm::vec2(1 / 16.f * 14, 1 / 16.f * 1));

	sprite->setAnimationParams(FALLING_RIGHT3, 1, false);
	sprite->addKeyframe(FALLING_RIGHT3, glm::vec2(1 / 16.f * 14, 1 / 16.f * 1));


	sprite->setAnimationParams(FALL_ANIMATION, 8, false, 2);
	sprite->addKeyframe(FALL_ANIMATION, glm::vec2(1 / 16.f * 11, 1 / 16.f * 1));


	sprite->setAnimationParams(FALL_ANIMATION_MIRROR, 8, true, 2);
	sprite->addKeyframe(FALL_ANIMATION_MIRROR, glm::vec2(1 / 16.f * 11, 1 / 16.f * 1));


	sprite->setAnimationParams(FALL_ANIMATION_RIGHT, 8, false, 2);
	sprite->addKeyframe(FALL_ANIMATION_RIGHT, glm::vec2(1 / 16.f * 14, 1 / 16.f * 1));


	sprite->setAnimationParams(FALL_ANIMATION_LEFT, 8, true, 1);
	sprite->addKeyframe(FALL_ANIMATION_LEFT, glm::vec2(1 / 16.f * 14, 1 / 16.f * 1));



	sprite->setAnimationParams(CROUCH_LEFT, 8, true);
	sprite->addKeyframe(CROUCH_LEFT, glm::vec2(1 / 8.f * 4, 0.0f));

	sprite->setAnimationParams(CROUCH_RIGHT, 8, false);
	sprite->addKeyframe(CROUCH_RIGHT, glm::vec2(1 / 8.f * 3, 0.0f));

	sprite->changeAnimation(STAND_RIGHT);
}


void Player::update(int deltaTime)
{	
	sprite->update(deltaTime);
	
	if(bJumping)
	{
		//Check if next jump frame is safe, before advancing
		int nextJumpAngle = jumpAngle + JUMP_ANGLE_STEP;

		int nextY = int(startY - JUMP_HEIGHT * sin(3.14159f * nextJumpAngle / 180.f));
		if (map->collisionMoveUp(glm::ivec2(posCharacter.x + colliderOffset.x, nextY + colliderOffset.y), colliderSize, true)) {
			jumpAngle = (180 - jumpAngle);
		}

		if(jumpAngle == 180)
		{
			bJumping = false;
			posCharacter.y = startY;
		}
		else
		{
			//Advance jumpAngle
			jumpAngle += JUMP_ANGLE_STEP;

			posCharacter.y = int(startY - JUMP_HEIGHT * sin(3.14159f * nextJumpAngle / 180.f));

			checkCollisionWithPlatform();

			if (jumpAngle > 90) {
				int tileCol = map->collisionMoveDown(posCharacter + colliderOffset, colliderSize, &posCharacter.y);
				bJumping = tileCol == TILE_NOT_SOLID;
			}
		}
		//Cambiar animaciones de salto
		if (jumpAngle < 90) {
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
		else if (jumpAngle < 135) {
			if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT)) {
				rightSight = true;
				sprite->changeAnimation(FALLING_RIGHT1);
			}
			else if (Game::instance().getSpecialKey(GLUT_KEY_LEFT)) {
				rightSight = false;
				sprite->changeAnimation(FALLING_LEFT1);
			}
			else {
				if (rightSight) sprite->changeAnimation(FALLING1);
				else sprite->changeAnimation(FALLING_MIRROR1);
			}
		}
		else if (jumpAngle > 135 && jumpAngle < 150) {
			if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT)) {
				rightSight = true;
				sprite->changeAnimation(FALLING_RIGHT2);
			}
			else if (Game::instance().getSpecialKey(GLUT_KEY_LEFT)) {
				rightSight = false;
				sprite->changeAnimation(FALLING_LEFT2);
			}
			else {
				if (rightSight) sprite->changeAnimation(FALLING2);
				else sprite->changeAnimation(FALLING_MIRROR2);
			}
		}
		else if (jumpAngle > 150) {
			if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT)) {
				rightSight = true;
				sprite->changeAnimation(FALLING_RIGHT3);
			}
			else if (Game::instance().getSpecialKey(GLUT_KEY_LEFT)) {
				rightSight = false;
				sprite->changeAnimation(FALLING_LEFT3);
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
		checkCollisionWithPlatform();
		if(map->collisionMoveDown(posCharacter + colliderOffset, colliderSize, &posCharacter.y) != 0)
		{
			if(Game::instance().getSpecialKey(GLUT_KEY_UP))
			{
				bJumping = true;
				jumpAngle = 0;
				startY = posCharacter.y;
			}
			if (Game::instance().getSpecialKey(GLUT_KEY_LEFT))
			{
				if (sprite->animation() != MOVE_LEFT) {
					sprite->changeAnimation(MOVE_LEFT);
					rightSight = false;
				}
				posCharacter.x -= moveSpeed;
				if (map->collisionMoveLeft(posCharacter + colliderOffset, colliderSize, bJumping))
				{
					posCharacter.x += moveSpeed;
					sprite->changeAnimation(STAND_LEFT);
				}
			}
			else if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT))
			{
				if (sprite->animation() != MOVE_RIGHT) {
					sprite->changeAnimation(MOVE_RIGHT);
					rightSight = true;
				}
				posCharacter.x += moveSpeed;
				if (map->collisionMoveRight(posCharacter + colliderOffset, colliderSize, bJumping))
				{
					posCharacter.x -= moveSpeed;
					sprite->changeAnimation(STAND_RIGHT);
				}
			}
			else if (Game::instance().getSpecialKey(GLUT_KEY_DOWN)) {
				if (rightSight && sprite->animation() != CROUCH_RIGHT) {
					sprite->changeAnimation(CROUCH_RIGHT);
				}
				else if (!rightSight && sprite->animation() != CROUCH_LEFT) {
					sprite->changeAnimation(CROUCH_LEFT);
				}
			}
			else
			{
				if (rightSight && sprite->animation() != STAND_RIGHT) sprite->changeAnimation(STAND_RIGHT);
				else if (!rightSight && sprite->animation() != STAND_LEFT) sprite->changeAnimation(STAND_LEFT);
			}
		}
		else {
			if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT)) {
				rightSight = true;
				sprite->changeAnimation(FALL_ANIMATION_RIGHT);
			}
			else if (Game::instance().getSpecialKey(GLUT_KEY_LEFT)) {
				rightSight = false;
				sprite->changeAnimation(FALL_ANIMATION_LEFT);
			}
			else {
				if (rightSight) sprite->changeAnimation(FALL_ANIMATION);
				else sprite->changeAnimation(FALL_ANIMATION_MIRROR);
			}
		}
		
	}

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posCharacter.x), float(tileMapDispl.y + posCharacter.y)));
}

void Player::checkCollisionWithPlatform() {
	int tileSize = map->getTileSize();
	for (int i = 0; i < 2; ++i) {
		int offset = (i == 0) ? 0 : colliderSize.x - 1;
		int tileCol = map->collisionMoveDown(glm::ivec2(posCharacter.x + colliderOffset.x + offset, posCharacter.y + colliderOffset.y), glm::ivec2(1, colliderSize.y), nullptr);
		if (tileCol == TILE_PLATFORM) {
			map->modifyTileMap((posCharacter.y + colliderOffset.y + colliderSize.y) / tileSize, (posCharacter.x + colliderOffset.x + offset) / tileSize, /*4 * 8 + 5*/ -16);
			map->reduceNumberOfPlatforms();
		}
	}
}


bool Player::isHurted() {
	return hurted;
}

bool Player::checkCollisionWithRect(const glm::ivec2& leftTop, const glm::ivec2& rightBottom, int type) {
	glm::ivec2 p0(posCharacter.x + colliderOffset.x, posCharacter.y + colliderOffset.y);
	glm::ivec2 p1(posCharacter.x + colliderOffset.x + colliderSize.x, posCharacter.y + colliderOffset.y + colliderSize.y);
	if (p0.x < rightBottom.x && p1.x > leftTop.x && p0.y < rightBottom.y && p1.y > leftTop.y) {
		if (type == TYPE_ENEMY) hurted = true;
		return true;
	}
	return false;
}


