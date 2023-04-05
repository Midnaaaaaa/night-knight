#ifndef _SPRITE_INCLUDE
#define _SPRITE_INCLUDE


#include <vector>
#include <glm/glm.hpp>
#include "Texture.h"
#include "ShaderProgram.h"
#include "AnimKeyframes.h"
#include "Effect.h"
#include <stack>


// This class is derived from code seen earlier in TexturedQuad but it is also
// able to manage animations stored as a spritesheet. 

enum Effects {
	EFFECT_BLINK, EFFECT_SHAKE, EFFECT_SIN_Y, EFFECT_SPAWN, EFFECT_INVIS, EFFECT_BH
};

class Sprite
{

private:
	Sprite(const glm::vec2& tileMapPos, const glm::vec2 &quadSize, const glm::vec2 &sizeInSpritesheet, Texture *spritesheet, ShaderProgram *program);
	//~Sprite();
public:
	// Textured quads can only be created inside an OpenGL context
	static Sprite *createSprite(const glm::vec2& tileMapPos, const glm::vec2 &quadSize, const glm::vec2 &sizeInSpritesheet, Texture *spritesheet, ShaderProgram *program);

	void update(int deltaTime, bool freeze = false);
	void render(glm::vec4 color = glm::vec4(1.f)) const;
	void render(const Texture& tex) const;
	void free();

	void setNumberAnimations(int nAnimations);
	void setAnimationParams(int animId, int keyframesPerSec, bool mirror, int loopStart = 0, int nextAnimation = -1);
	void addKeyframe(int animId, const glm::vec2 &frame);
	void changeAnimation(int animId, int startFrame = 0);
	int animation() const;
	void setDisplacement(const glm::vec2& displacement);

	void setPosition(const glm::vec2 &pos);

	glm::vec2 getPosition();
	glm::vec2 getSpriteSize();

	void addEffect(int id, int duration, int delay = 0);
	void addEffect(int id, int duration, const glm::ivec2& point, int delay = 0);
	void refreshFreezeEffect(int duration, int delay);
	int currentEffectId();

private:
	void updateTimers(int deltaTime);

	Texture *texture;
	ShaderProgram *shaderProgram;
	GLuint vao;
	GLuint vbo;
	GLint posLocation, texCoordLocation;
	glm::vec2 position, tileMapDispl;
	int currentAnimation, currentKeyframe;
	float timeAnimation;
	glm::vec2 texCoordDispl;
	vector<AnimKeyframes> animations;
	glm::vec2 spriteSize;
	glm::vec2 sizeOfEachTile;

	deque<Effect> effectStack;


};


#endif // _SPRITE_INCLUDE

