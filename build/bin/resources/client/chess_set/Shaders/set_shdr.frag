#version 450 core
out vec4 FragColor;

in vec3 Light_Color;
in vec3 col;

void main()
{
	FragColor.xyz = col * Light_Color;
	FragColor.w = 1.0f;
}
