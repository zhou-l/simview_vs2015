#version 330 compatibility

uniform sampler2D  tex2D;
uniform bool       flipY;
in vec4 texc;

out vec4 fColor;

void main()
{
	vec2 samplePos = (flipY)? vec2(texc.x, 1.0 - texc.y) : vec2(texc.xy);
	float val =   texture(tex2D, samplePos).r;
	if (val == 0.0)
		fColor = vec4(0.3, 0.3, 0.3, 1);
	else
		//fColor = vec4(samplePos, 0, 1);
		fColor = vec4(val, 0, 0, 1);
	//fColor = vec4(texc.xy, val,1);
}
