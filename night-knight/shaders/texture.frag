#version 330

uniform vec4 color;
uniform sampler2D tex;

uniform int effectTimer;
uniform int effectId;

in vec2 texCoordFrag;
out vec4 outColor;

void main()
{
	// Discard fragment if texture sample has alpha < 0.5
	// otherwise compose the texture sample with the fragment's interpolated color

	vec2 texCoord = texCoordFrag;

	if (effectId == 0) { //Blink
		if ((effectTimer % 100) < 50)
			discard;
	}

	vec4 texColor = texture(tex, texCoord);
	if(texColor.a < 0.5f)
		discard;
	outColor = color * texColor;
}

