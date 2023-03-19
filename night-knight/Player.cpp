#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Player.h"
#include "Game.h"


#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT (96 + 5)/2
#define FALL_STEP 4
#define PLAYER_OFFSET 8


enum PlayerAnims
{
	STAND_LEFT, STAND_RIGHT, MOVE_LEFT, MOVE_RIGHT, CROUCH_LEFT, CROUCH_RIGHT
};


void Player::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	bJumping = false;
	spritesheet.loadFromFile("images/simon-sprite.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(1/8.f, 1/4.f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(6);

		sprite->setAnimationSpeed(STAND_LEFT, 8);
		sprite->addKeyframe(STAND_LEFT, glm::vec2(1/8.f * 7, 0.f));
		
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
	rightSight = true;
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
	
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
		posPlayer.x -= 2;
		if(map->collisionMoveLeft(glm::ivec2(posPlayer.x + PLAYER_OFFSET, posPlayer.y), playerSize, bJumping))
		{
			posPlayer.x += 2;
			sprite->changeAnimation(STAND_LEFT);
		}
	}
	else if(Game::instance().getSpecialKey(GLUT_KEY_RIGHT))
	{
		if (sprite->animation() != MOVE_RIGHT) {
			sprite->changeAnimation(MOVE_RIGHT);
			rightSight = true;
		}
		posPlayer.x += 2;
		if(map->collisionMoveRight(glm::ivec2(posPlayer.x + PLAYER_OFFSET, posPlayer.y), playerSize, bJumping))
		{
			posPlayer.x -= 2;
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
		if (map->collisionMoveUp(glm::ivec2(posPlayer.x + PLAYER_OFFSET, nextY), playerSize)) {
			jumpAngle = (180 - jumpAngle);
		}

		if(jumpAngle == 180)
		{
			bJumping = false;
			posPlayer.y = startY;
		}
		else
		{
			//Advance jumpAngle
			jumpAngle += JUMP_ANGLE_STEP;

			posPlayer.y = int(startY - JUMP_HEIGHT * sin(3.14159f * nextJumpAngle / 180.f));

			checkCollisionWithPlatform();

			if (jumpAngle > 90) {
				int tileCol = map->collisionMoveDown(glm::ivec2(posPlayer.x + PLAYER_OFFSET, posPlayer.y), playerSize, &posPlayer.y);
				bJumping = tileCol == TILE_NOT_SOLID;
			}
		}
	}
	else
	{
		posPlayer.y += FALL_STEP;
		checkCollisionWithPlatform();
		if(map->collisionMoveDown(glm::ivec2(posPlayer.x + PLAYER_OFFSET, posPlayer.y), playerSize, &posPlayer.y) != 0)
		{
			if(Game::instance().getSpecialKey(GLUT_KEY_UP))
			{
				bJumping = true;
				jumpAngle = 0;
				startY = posPlayer.y;
			}
		}
	}

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

void Player::checkCollisionWithPlatform() {
	int tileSize = map->getTileSize();
	for (int i = 0; i < 2; ++i) {
		int dummy = posPlayer.y;
		int offset = (i == 0) ? 0 : playerSize.x - 1;
		int tileCol = map->collisionMoveDown(glm::ivec2(posPlayer.x + PLAYER_OFFSET + offset, posPlayer.y), glm::ivec2(1, playerSize.y), &dummy);
		if (tileCol == TILE_PLATFORM) {
			map->modifyTileMap(posPlayer.y / tileSize + 2, (posPlayer.x + PLAYER_OFFSET + offset) / tileSize, 4 * 8 + 5);
		}
	}
}

void Player::render()
{
	sprite->render();
}

void Player::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}

void Player::setPosition(const glm::vec2 &pos)
{
	posPlayer = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}




