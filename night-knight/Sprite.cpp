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

void Sprite::update(int deltaTime)
{
	if(currentAnimation >= 0)
	{
		timeAnimation += deltaTime;
		while(timeAnimation > animations[currentAnimation].millisecsPerKeyframe)
		{
			timeAnimation -= animations[currentAnimation].millisecsPerKeyframe;
			

			int frameCount = animations[currentAnimation].keyframeDispl.size();
			int loopStart = animations[currentAnimation].loopStart;
			loopStart = (loopStart < frameCount) ? loopStart : frameCount; //Para que no se pase
			if (frameCount > 15) {
				cout << "hola";
			}
			currentKeyframe++;
			if (currentKeyframe >= frameCount) currentKeyframe = loopStart;
			
		}
		texCoordDispl = animations[currentAnimation].keyframeDispl[currentKeyframe];
	}
}

void Sprite::render() const
{
	glm::mat4 modelview = glm::translate(glm::mat4(1.0f), glm::vec3(position.x + tileMapDispl.x, position.y + tileMapDispl.y, 0.f));
	shaderProgram->setUniformMatrix4f("modelview", modelview);
	shaderProgram->setUniform2f("texCoordDispl", texCoordDispl.x, texCoordDispl.y);
	glEnable(GL_TEXTURE_2D);
	texture->use();
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

void Sprite::setAnimationParams(int animId, int keyframesPerSec, bool mirror, int loopStart)
{
	if (animId < int(animations.size())) {
		animations[animId].millisecsPerKeyframe = 1000.f / keyframesPerSec;
		animations[animId].loopStart = loopStart;
		animations[animId].mirror = mirror;
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

void Sprite::changeAnimation(int animId)
{
	if(animId < int(animations.size()))
	{
		currentAnimation = animId;
		currentKeyframe = 0;
		timeAnimation = 0.f;
		texCoordDispl = animations[animId].keyframeDispl[0];
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