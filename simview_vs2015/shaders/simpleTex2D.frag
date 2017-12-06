#version 330

uniform sampler2D  tex2D;
varying mediump vec4 texc;

out highp vec4 fColor;

void main()
{

	fColor = texture2D(tex2D, texc.xy);
}
