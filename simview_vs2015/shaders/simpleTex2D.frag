#version 330 compatibility

uniform sampler2D  tex2D;
in vec4 texc;

out vec4 fColor;

void main()
{
	vec2 samplePos = vec2(texc.xy);
	float val =   texture(tex2D, samplePos).r;
	if (val == 0.0)
		fColor = vec4(texc.xy, 0.0, 1);
	else
		//fColor = vec4(samplePos, 0, 1);
		fColor = vec4(val, 0, 0, 1);
	//fColor = vec4(texc.xy, val,1);
}
