#version 330

uniform vec2 texCoordDispl;

uniform int effectTimer;
uniform int effectId;
uniform int effectDuration;

uniform vec2 center;


in vec2 position;
in vec2 texCoord;
out vec2 texCoordFrag;

const float PI = 3.1415926535897932384626433832795;


void main()
{
    texCoordFrag = texCoord;
	vec2 pos = position;
    gl_Position = vec4(pos, 0.0, 1.0);
}

