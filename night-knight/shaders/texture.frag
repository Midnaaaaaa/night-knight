#version 330

uniform vec4 color;
uniform sampler2D tex;

uniform int effectTimer;
uniform int effectId;

in vec2 texCoordFrag;
in float alpha;
out vec4 outColor;

const float PI = 3.1415926535897932384626433832795;

const int effect3Duration = 1000;

void main()
{
	// Discard fragment if texture sample has alpha < 0.5
	// otherwise compose the texture sample with the fragment's interpolated color


	//if(mod(int(gl_FragCoord.y),4) == 0) discard;

	vec2 texCoord = texCoordFrag;

	if (effectId == 0) { //Blink
		if ((effectTimer % 100) < 50)
			discard;
	}
    else if (effectId == 3) { //?
        texCoord.x = texCoord.x + sin((effectTimer)/1000.f*2*PI + int( 100*texCoord.y*2*PI )) * 0.01 * sin(-effectTimer * (PI/(effect3Duration*1.f)));
    }
	else if (effectId == 4) {
		discard;
	}

	vec4 texColor = texture(tex, texCoord);
	if(texColor.a < 0.5f)
		discard;

    texColor.a = 0.1f;
	outColor = color * texColor;
}

