#include "Character.h"


enum PlayerAnims
{
	STAND_LEFT, STAND_RIGHT, MOVE_LEFT, MOVE_RIGHT, CROUCH_LEFT, CROUCH_RIGHT
};


void Character::init(const glm::ivec2& tileMapPos, string spriteFile, const glm::ivec2& colliderSize, const glm::ivec2& colliderOffset, const glm::ivec2& pixelSize, const glm::vec2 &texSize, ShaderProgram& shaderProgram)
{
	this->colliderSize = colliderSize;
	this->colliderOffset = colliderOffset;
	bJumping = false;
	spritesheet.loadFromFile(spriteFile, TEXTURE_PIXEL_FORMAT_RGBA);
	//sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(1 / 8.f, 1 / 4.f), &spritesheet, &shaderProgram);
	sprite = Sprite::createSprite(pixelSize, texSize, &spritesheet, &shaderProgram);
	loadAnimations();
	rightSight = true;
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posCharacter.x), float(tileMapDispl.y + posCharacter.y)));

}


void Character::render() {
	sprite->render();
}

void Character::setTileMap(TileMap* tileMap)
{
	map = tileMap;
}

void Character::setPosition(const glm::vec2& pos)
{
	posCharacter = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posCharacter.x), float(tileMapDispl.y + posCharacter.y)));
}


