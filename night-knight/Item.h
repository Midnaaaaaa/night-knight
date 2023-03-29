#pragma once
#include "Sprite.h"

using namespace std;


// AnimKeyframes contains all information related to a single animation.
// These are the animation speed measured by millisecsPerKeyframe
// and texture coordinates for all keyframes.


struct Item
{
	Sprite* sprite;
	int id;
};
