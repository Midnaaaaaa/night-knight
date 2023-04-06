#version 330

uniform vec4 color;
uniform sampler2D tex;

uniform int effectTimer;
uniform int effectId;
uniform int effectDuration;

uniform vec2 center;

in vec2 texCoordFrag;
out vec4 outColor;

const float PI = 3.1415926535897932384626433832795;

void main()
{
	// Discard fragment if texture sample has alpha < 0.5
	// otherwise compose the texture sample with the fragment's interpolated color

	vec2 texCoord = texCoordFrag;
    vec4 fColor = color;
    float gray = 1;
    if (effectId == 0) { //Fade
        gray = (effectTimer/float(effectDuration));
    }
    //texCoord.x = texCoord.x + sin((effectTimer)/1000.f*2*PI + int( 10*texCoord.y*2*PI )) * 0.01;

    vec4 texColor = texture(tex, texCoord);

	outColor = (color*gray) * texColor;
}

