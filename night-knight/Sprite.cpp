#include <GL/glew.h>
#include <GL/gl.h>
#include <glm/gtc/matrix_transform.hpp>
#include "Sprite.h"
#include <iostream>


Sprite *Sprite::createSprite(const glm::vec2& tileMapPos, const glm::vec2 &quadSize, const glm::vec2 &sizeInSpritesheet, Texture *spritesheet, ShaderProgram *program)
{
	Sprite *quad = new Sprite(tileMapPos, quadSize, sizeInSpritesheet, spritesheet, program);

	return quad;
}


Sprite::Sprite(const glm::vec2& tileMapPos, const glm::vec2 &quadSize, const glm::vec2 &sizeInSpritesheet, Texture *spritesheet, ShaderProgram *program)
{
	float vertices[24] = {0.f, 0.f, 0.f, 0.f, 
												quadSize.x, 0.f, sizeInSpritesheet.x, 0.f, 
												quadSize.x, quadSize.y, sizeInSpritesheet.x, sizeInSpritesheet.y, 
												0.f, 0.f, 0.f, 0.f, 
												quadSize.x, quadSize.y, sizeInSpritesheet.x, sizeInSpritesheet.y, 
												0.f, quadSize.y, 0.f, sizeInSpritesheet.y};

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(float), vertices, GL_STATIC_DRAW);
	posLocation = program->bindVertexAttribute("position", 2, 4*sizeof(float), 0);
	texCoordLocation = program->bindVertexAttribute("texCoord", 2, 4*sizeof(float), (void *)(2*sizeof(float)));
	texture = spritesheet;
	shaderProgram = program;
	currentAnimation = -1;
	spriteSize = quadSize;
	tileMapDispl = tileMapPos;
	position = glm::vec2(0.f);
	sizeOfEachTile = sizeInSpritesheet;
}

//Sprite::~Sprite()
//{
//	free();
//}

void Sprite::update(int deltaTime, bool freeze)
{
	updateTimers(deltaTime);
	if(currentAnimation >= 0 && !freeze)
	{
		timeAnimation += deltaTime;
		while(timeAnimation > animations[currentAnimation].millisecsPerKeyframe)
		{
			timeAnimation -= animations[currentAnimation].millisecsPerKeyframe;
			

			int frameCount = animations[currentAnimation].keyframeDispl.size();
			int loopStart = animations[currentAnimation].loopStart;
			loopStart = (loopStart < frameCount) ? loopStart : frameCount; //Para que no se pase

			currentKeyframe++;
			if (currentKeyframe >= frameCount) {
				int nextAnim = animations[currentAnimation].nextAnimation;
				if (nextAnim == -1)
					currentKeyframe = loopStart;
				else {
					changeAnimation(nextAnim);
				}

			}
			
		}
		texCoordDispl = animations[currentAnimation].keyframeDispl[currentKeyframe];
	}
}

void Sprite::render(glm::vec4 color) const
{
	int effectId = -1;
	int effectTimer = 0;
	int effectDuration = 1;
	glm::ivec2 dest;
	if (!effectStack.empty()) {
		const Effect& e = effectStack.back();
		effectId = e.id;
		if (e.timer < 0) {
			effectTimer = e.timer + e.duration; // [-duration 0] ---> [0 duration] 
		}
		else {
			effectTimer = e.timer;
		}
		effectDuration = e.duration;
		if (effectId == 5) {
			dest = e.point;
		}
	}

	/*
	//--------------------DEBUG-------------------------
	int shakeCount = 0;
	int menouno = 0;
	for (const Effect& e : effectStack) {
		if (e.id == EFFECT_SHAKE) ++shakeCount;
		if (e.id == -1) ++menouno;
		if (shakeCount > 1 || menouno > 1) {
			cout << endl;
		}
	}
	//--------------------DEBUG-------------------------
	*/

	glm::mat4 modelview = glm::translate(glm::mat4(1.0f), glm::vec3(position.x + tileMapDispl.x, position.y + tileMapDispl.y, 0.f));
	shaderProgram->use();
	shaderProgram->setUniformMatrix4f("modelview", modelview);
	shaderProgram->setUniform2f("texCoordDispl", texCoordDispl.x, texCoordDispl.y);
	shaderProgram->setUniform1i("effectId", effectId);
	shaderProgram->setUniform1i("effectTimer", effectTimer);
	shaderProgram->setUniform1i("effectDuration", effectDuration);
	shaderProgram->setUniform4f("color", color.r, color.g, color.b, color.a);
	shaderProgram->setUniform2f("doorPos", dest.x + tileMapDispl.x, dest.y + tileMapDispl.y);

	glEnable(GL_TEXTURE_2D);
	texture->use();
	glBindVertexArray(vao);
	glEnableVertexAttribArray(posLocation);
	glEnableVertexAttribArray(texCoordLocation);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisable(GL_TEXTURE_2D);
}

//Render para texto, sin cambiar los uniforms porque ya se ha encargado la clase Text
void Sprite::render(const Texture& tex) const
{
	glEnable(GL_TEXTURE_2D);
	tex.use();
	glBindVertexArray(vao);
	glEnableVertexAttribArray(posLocation);
	glEnableVertexAttribArray(texCoordLocation);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisable(GL_TEXTURE_2D);
}

void Sprite::free()
{
	glDeleteBuffers(1, &vbo);
}

void Sprite::setNumberAnimations(int nAnimations)
{
	animations.clear();
	animations.resize(nAnimations);
}

void Sprite::setAnimationParams(int animId, int keyframesPerSec, bool mirror, int loopStart, int nextAnimation)
{
	if (animId < int(animations.size())) {
		animations[animId].millisecsPerKeyframe = 1000.f / keyframesPerSec;
		animations[animId].loopStart = loopStart;
		animations[animId].mirror = mirror;
		animations[animId].nextAnimation = nextAnimation;
	}
}

void Sprite::addKeyframe(int animId, const glm::vec2 &displacement)
{
	if (animId < int(animations.size())) {
		if (animations[animId].mirror) {
			animations[animId].keyframeDispl.push_back(glm::vec2(-displacement.x - sizeOfEachTile.x, displacement.y));
		}
		else animations[animId].keyframeDispl.push_back(displacement);
	}
}

void Sprite::changeAnimation(int animId, int startFrame)
{
	if(animId < int(animations.size()))
	{
		currentAnimation = animId;
		currentKeyframe = startFrame;
		timeAnimation = 0.f;
		texCoordDispl = animations[animId].keyframeDispl[startFrame];
	}
}

int Sprite::animation() const
{
	return currentAnimation;
}

void Sprite::setDisplacement(const glm::vec2& displacement) {
	this->texCoordDispl = displacement;
}

void Sprite::setPosition(const glm::vec2 &pos)
{
	position = pos;
}


glm::vec2 Sprite::getPosition() {
	return position;
}



glm::vec2 Sprite::getSpriteSize() { return spriteSize; }


void Sprite::addEffect(int id, int duration, int delay) {
	Effect e;
	e.id = id;
	e.timer = duration;
	e.duration = duration;

	effectStack.push_back(e);

	if (delay > 0) {
		Effect delayEffect;
		delayEffect.id = -1;
		delayEffect.timer = delay;

		effectStack.push_back(delayEffect);
	}
}

void Sprite::addEffect(int id, int duration, const glm::ivec2& point, int delay) {
	Effect e;
	e.id = id;
	e.point = point;
	e.timer = duration;
	e.duration = duration;
	if (duration < 0)
		e.duration = -duration;

	effectStack.push_back(e);

	if (delay > 0) {
		Effect delayEffect;
		delayEffect.id = -1;
		delayEffect.timer = delay;

		effectStack.push_back(delayEffect);
	}
}

void Sprite::refreshFreezeEffect(int duration, int delay) {
	//Cleanup stack

	//Encuentra indice
	if (!effectStack.empty()) {
		int i = 0;

		int elemToDelete = 1;
		while (i < effectStack.size() - 1) {
			if (effectStack[i].id == EFFECT_SHAKE) {
				if (i + 1 <= effectStack.size() && effectStack[i + 1].id == -1) elemToDelete = 2;
				break;
			}
			++i;
		}

		//Mueve
		for (; i < effectStack.size() - elemToDelete; ++i) {
			effectStack[i] = effectStack[i + elemToDelete];
		}

		while (elemToDelete-- > 0)
			effectStack.pop_back();
	}

	addEffect(EFFECT_SHAKE, duration, delay);
}

int Sprite::currentEffectId() {
	if (!effectStack.empty()) {
		return effectStack.back().id;
	}
}

void Sprite::updateTimers(int deltaTime) {
	if (!effectStack.empty()) {
		Effect& e = effectStack.back();
		if (e.timer > 0) {
			e.timer -= deltaTime;
			if (e.timer <= 0) {
				effectStack.pop_back();
			}
		}
		else if (e.timer < 0) {
			e.timer += deltaTime;
			if (e.timer >= 0) {
				effectStack.pop_back();
			}
		}
	}
}
