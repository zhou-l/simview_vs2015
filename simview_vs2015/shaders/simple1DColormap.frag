#version 330

in highp vec4 vColor;
out highp vec4 fColor;

const vec3 divColN0 = vec3(103.0f, 0.0f, 31.0f)/255.0f; // red
const vec3 divColN1 = vec3(247.0f, 247.0f, 247.0f)/255.0f; // white
const vec3 divColN2 = vec3(5.0f, 48.0f, 97.0f) / 255.0f; // blue

vec3 lerpColor(float a, vec3 C1, vec3 C2)
{

	vec3 C = (1.0 - a) * C1 + a * C2;
	return C;
}

void main()
{
	float fval = vColor.x;

	float minVal = -7.0f;
	float maxVal = 10.0f;
	float val = (fval - minVal) / (maxVal - minVal);


	//fColor.rgb = lerpColor(val, divColN2, divColN1);
	if (val < 0.5)
	{
		fColor.rgb = lerpColor(2 * val, divColN2, divColN1);
	}
	else
	{
		fColor.rgb = lerpColor(2 * val - 1.0, divColN1, divColN0);
	}
}
