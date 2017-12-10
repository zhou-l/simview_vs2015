#version 330 compatibility

uniform sampler2D  tex2D;
in vec4 texc;


void main()
{
	vec2 samplePos = vec2(texc.xy);
	vec3 val =   texture(tex2D, samplePos).xyz;
	if (length(val) > 0.0)
		gl_FragColor = vec4(1.1, 0.2, 0.8, 1);
	else
		gl_FragColor = vec4(samplePos, 0, 1);
	//fColor = vec4(texc.xy, val,1);
}
