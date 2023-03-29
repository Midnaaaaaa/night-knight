#include "Character.h"

void Character::init(const glm::ivec2& tileMapPos, bool rightSight, string spriteFile, const glm::ivec2& colliderSize, const glm::ivec2& colliderOffset, const glm::ivec2& pixelSize, const glm::vec2 &texSize, ShaderProgram& shaderProgram)
{
	this->moveSpeed = 1;
	this->rightSight = rightSight;
	this->colliderSize = colliderSize;
	this->colliderOffset = colliderOffset;
	this->spriteSize = pixelSize;
	bJumping = false;
	spritesheet.loadFromFile(spriteFile, TEXTURE_PIXEL_FORMAT_RGBA);
	//sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(1 / 8.f, 1 / 4.f), &spritesheet, &shaderProgram);
	sprite = Sprite::createSprite(tileMapDispl, pixelSize, texSize, &spritesheet, &shaderProgram);
	loadAnimations();
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posCharacter.x), float(tileMapDispl.y + posCharacter.y)));

	this->freezeTimer = 0;
	this->effectDelay = 0;
	this->effectTimer = 0;
	this->effectId = -1;

}

Character::~Character() {
	sprite->free();
}


void Character::render() {
	sprite->render((effectDelay <= 0) ? effectId : -1, effectTimer);
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

void Character::setSpeed(int speed) {
	this->moveSpeed = speed;
	this->moveSpeedBase = speed;
}

glm::ivec2 Character::getPosition() const{
	return posCharacter;
}

glm::ivec2 Character::getSize() const{
	return colliderSize;
}

void Character::freeze(int milisec, bool tremolar) {
	freezeTimer = milisec;
	if(moveSpeed != 0) moveSpeedBase = moveSpeed;
	moveSpeed = 0;
	if (tremolar) {
		effect(EFFECT_SHAKE, 2000, milisec - 2000);
	}
}

void Character::effect(int id, int duration, int delay) {
	this->effectId = id;
	this->effectTimer = duration;
	this->effectDelay = delay;
}

void Character::updateTimers(int deltaTime) {
	if (freezeTimer > 0) {
		freezeTimer -= deltaTime;
		if (freezeTimer < 0) {
			moveSpeed = moveSpeedBase;
			freezeTimer = 0;
		}
	}

	if (effectDelay > 0) {
		effectDelay -= deltaTime;
		if (effectDelay < 0) {
			effectDelay = 0;
		}
	}

	//Si no queda Delay, empieza el effectTimer
	if (effectDelay <= 0 && effectTimer > 0) {
		effectTimer -= deltaTime;
		if (effectTimer < 0) {
			effectTimer = 0;
			effectId = -1;
		}
	}
}