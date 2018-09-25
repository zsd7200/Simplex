#version 330

in vec3 Color;

uniform vec3 SolidColor = vec3(-1,-1,-1);

out vec4 Fragment;

uniform int Complimentary;

void main()
{
	Fragment = vec4(Color,1);
	
	if(SolidColor.r != -1.0 && SolidColor.g != -1.0 && SolidColor.b != -1.0)
		Fragment = vec4(SolidColor, 1);
	
	// if complimentary is active, make colors complimentary
	if (Complimentary == 1)
	{
		Fragment.x = 1.0f - Fragment.x;
		Fragment.y = 1.0f - Fragment.y;
		Fragment.z = 1.0f - Fragment.z;
	}
		
	return;
}