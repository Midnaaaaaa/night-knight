#pragma once
class Scene
{
public:

	virtual void init() = 0;
	virtual void update(int deltaTime) = 0;
	virtual void render() = 0;

};

