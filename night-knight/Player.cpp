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


void Player::loadAnimations() {
	sprite->setNumberAnimations(6);

	sprite->setAnimationSpeed(STAND_LEFT, 8);
	sprite->addKeyframe(STAND_LEFT, glm::vec2(1 / 8.f * 7, 0.f));

	sprite->setAnimationSpeed(STAND_RIGHT, 8);
	sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.0f, 0.0f));

	sprite->setAnimationSpeed(MOVE_LEFT, 10);
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(1 / 8.f * 7, 0.0f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(1 / 8.f * 6, 0.0f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(1 / 8.f * 5, 0.0f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(1 / 8.f * 6, 0.0f));

	sprite->setAnimationSpeed(MOVE_RIGHT, 10);
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.0f, 0.0f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(1 / 8.f * 1, 0.0f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(1 / 8.f * 2, 0.0f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(1 / 8.f * 1, 0.0f));

	sprite->setAnimationSpeed(CROUCH_LEFT, 8);
	sprite->addKeyframe(CROUCH_LEFT, glm::vec2(1 / 8.f * 4, 0.0f));

	sprite->setAnimationSpeed(CROUCH_RIGHT, 8);
	sprite->addKeyframe(CROUCH_RIGHT, glm::vec2(1 / 8.f * 3, 0.0f));

	sprite->changeAnimation(1);
}


void Player::update(int deltaTime)
{	
	sprite->update(deltaTime);
	if(Game::instance().getSpecialKey(GLUT_KEY_LEFT))
	{
		if (sprite->animation() != MOVE_LEFT) {
			sprite->changeAnimation(MOVE_LEFT);
			rightSight = false;
		}
		posCharacter.x -= 2;
		if(map->collisionMoveLeft(glm::ivec2(posCharacter.x + colliderOffset.x, posCharacter.y), colliderSize, bJumping))
		{
			posCharacter.x += 2;
			sprite->changeAnimation(STAND_LEFT);
		}
	}
	else if(Game::instance().getSpecialKey(GLUT_KEY_RIGHT))
	{
		if (sprite->animation() != MOVE_RIGHT) {
			sprite->changeAnimation(MOVE_RIGHT);
			rightSight = true;
		}
		posCharacter.x += 2;
		if(map->collisionMoveRight(glm::ivec2(posCharacter.x + colliderOffset.x, posCharacter.y), colliderSize, bJumping))
		{
			posCharacter.x -= 2;
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
		if(rightSight) sprite->changeAnimation(STAND_RIGHT);
		else sprite->changeAnimation(STAND_LEFT);
	}
	
	if(bJumping)
	{
		//Check if next jump frame is safe, before advancing
		int nextJumpAngle = jumpAngle + JUMP_ANGLE_STEP;

		int nextY = int(startY - JUMP_HEIGHT * sin(3.14159f * nextJumpAngle / 180.f));
		if (map->collisionMoveUp(glm::ivec2(posCharacter.x + colliderOffset.x, nextY), colliderSize, true)) {
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
				int tileCol = map->collisionMoveDown(glm::ivec2(posCharacter.x + colliderOffset.x, posCharacter.y), colliderSize, &posCharacter.y);
				bJumping = tileCol == TILE_NOT_SOLID;
			}
		}
	}
	else
	{
		posCharacter.y += FALL_STEP;
		checkCollisionWithPlatform();
		if(map->collisionMoveDown(glm::ivec2(posCharacter.x + colliderOffset.x, posCharacter.y), colliderSize, &posCharacter.y) != 0)
		{
			if(Game::instance().getSpecialKey(GLUT_KEY_UP))
			{
				bJumping = true;
				jumpAngle = 0;
				startY = posCharacter.y;
			}
		}
	}

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posCharacter.x), float(tileMapDispl.y + posCharacter.y)));
}

void Player::checkCollisionWithPlatform() {
	int tileSize = map->getTileSize();
	for (int i = 0; i < 2; ++i) {
		int dummy = posCharacter.y;
		int offset = (i == 0) ? 0 : colliderSize.x - 1;
		int tileCol = map->collisionMoveDown(glm::ivec2(posCharacter.x + colliderOffset.x + offset, posCharacter.y), glm::ivec2(1, colliderSize.y), &dummy);
		if (tileCol == TILE_PLATFORM) {
			map->modifyTileMap(posCharacter.y / tileSize + 2, (posCharacter.x + colliderOffset.x + offset) / tileSize, 4 * 8 + 5);
		}
	}
}


bool Player::isHurted() {
	return hurted;
}


bool Player::checkCollisionWithSprite(glm::ivec2 leftTop, glm::ivec2 rightBottom, int type) {
	glm::ivec2 p0(posCharacter.x + colliderOffset.x, posCharacter.y + colliderOffset.y);
	glm::ivec2 p1(posCharacter.x + colliderOffset.x + colliderSize.x, posCharacter.y + colliderOffset.y + colliderSize.y);
	if (p0.x < rightBottom.x && p1.x > leftTop.x && p0.y < rightBottom.y && p1.y > leftTop.y) {
		if (type == TYPE_ENEMY) hurted = true;
		return true;
	}
	return false;
}


