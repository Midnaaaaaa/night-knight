#version 330

uniform vec4 color;
uniform sampler2D tex;

uniform int effectTimer;
uniform int effectId;
uniform int effectDuration;

in vec2 texCoordFrag;
out vec4 outColor;

const float PI = 3.1415926535897932384626433832795;

void main()
{
	// Discard fragment if texture sample has alpha < 0.5
	// otherwise compose the texture sample with the fragment's interpolated color

	vec2 texCoord = texCoordFrag;

    //texCoord.x = texCoord.x + 0.5;

    vec4 texColor = texture(tex, texCoord);

	outColor = texColor;
}

