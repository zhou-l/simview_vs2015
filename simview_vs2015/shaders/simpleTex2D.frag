#version 330 compatibility

uniform sampler2D  tex2D;
in vec4 texc;

out vec4 fColor;

void main()
{

	float val =  10 * texture(tex2D, texc.xy).r;
	fColor = vec4(val, 0, 0, 1);
	fColor = vec4(texc.xy, val,1);
}
