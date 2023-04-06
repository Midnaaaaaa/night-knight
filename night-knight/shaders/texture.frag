#version 330

uniform vec4 color;
uniform sampler2D tex;

uniform int effectTimer;
uniform int effectId;
uniform int effectDuration;

in vec2 texCoordFrag;
in float alpha;
out vec4 outColor;

const float PI = 3.1415926535897932384626433832795;

uniform mat4 projection;
uniform int count;
uniform vec2 center[10];
uniform int radius[10];

uniform int WIDTH;
uniform int HEIGHT;


//const int effect3Duration = 1000;

void main()
{
	// Discard fragment if texture sample has alpha < 0.5
	// otherwise compose the texture sample with the fragment's interpolated color


	//if(mod(int(gl_FragCoord.y),4) == 0) discard;

	vec2 texCoord = texCoordFrag;

    
    float factor = 1;
    if (count > 1) { //dark level
        factor = 0;
        vec2 fragCoord;
        fragCoord.x = gl_FragCoord.x / WIDTH;
        fragCoord.y = gl_FragCoord.y / HEIGHT;
        for (int i = 0; i < count; ++i) {
            vec2 centerPos = ((projection * vec4(center[i], 0, 1)).xy + 1)/2;
            //factor = max(factor, smoothstep(0, 0.1f, distance(centerPos, fragCoord)));
            factor = min(1, factor + 1 - smoothstep(0, radius[i]/500.f, distance(centerPos, fragCoord)));
        }
    }


	if (effectId == 0) { //Blink
		if ((effectTimer % 100) < 50)
			discard;
	}
    else if (effectId == 3) { //?
        texCoord.x = texCoord.x + sin((effectTimer)/1000.f*2*PI + int( 100*texCoord.y*2*PI )) * 0.01 * sin(-effectTimer * (PI/(effectDuration*1.f)));
    }
	else if (effectId == 4) {
		discard;
	}

	vec4 texColor = texture(tex, texCoord);
	if(texColor.a < 0.5f)
		discard;

	outColor = color * (factor * texColor);
}

