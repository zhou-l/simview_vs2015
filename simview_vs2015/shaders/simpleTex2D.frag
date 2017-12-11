#version 330

uniform sampler2D  tex2D;
in mediump vec4 texc;

out vec4 fColor;

void main()
{
	vec2 samplePos = vec2(texc.xy);
	vec3 val =  100 *  texture(tex2D, samplePos).xyz;
	if (length(val) > 0.0)
		fColor = vec4(samplePos.x, 1.0 - samplePos.y, 0.8, 1);
	else
		fColor = vec4(samplePos, 0, 1);
	//fColor = vec4(texc.xy, val,1);
}
