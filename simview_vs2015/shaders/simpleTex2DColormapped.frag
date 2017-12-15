#version 330 compatibility

uniform sampler2D  tex2D;
uniform bool       flipY;
in vec4 texc;

out vec4 fColor;

const vec3 endColor = vec3(247, 251, 255)/255.0f;
const vec3 startColor = vec3(8, 69, 148)/255.0f;

vec3 lerpColor(float a, vec3 C1, vec3 C2)
{
	vec3 C = (1.0 - a) * C1 + a * C2;
	return C;
}

void main()
{
	vec2 samplePos = (flipY)? vec2(texc.x, 1.0 - texc.y) : vec2(texc.xy);
	float val =   texture(tex2D, samplePos).r;
	if (val == 0.0)
		fColor = vec4(0.3, 0.3, 0.3, 1);
	else
	{
		fColor = vec4(lerpColor(val * 100.0, startColor, endColor), 1);
	}
	//fColor = vec4(texc.xy, val,1);
}
