#version 330

uniform mat4 projection, modelview;
uniform vec2 texCoordDispl;

uniform int effectTimer;
uniform int effectId;


in vec2 position;
in vec2 texCoord;
out vec2 texCoordFrag;

const float PI = 3.1415926535897932384626433832795;

void main()
{

	vec2 pos = position;
	if (effectId == 1) { //Shake
		pos.x = pos.x + int((effectTimer % 100) < 50) * 5;
	}
    else if (effectId == 2) { //Sin-Y
        pos.y = pos.y + sin(effectTimer/1000.f*2*PI) * 6;
    }

	// Pass texture coordinates to access a given texture atlas
	texCoordFrag = texCoord + texCoordDispl;
	// Transform position from pixel coordinates to clipping coordinates
	gl_Position = projection * modelview * vec4(pos, 0.0, 1.0);
}

