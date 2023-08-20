#version 450 core
out vec4 FragColor;

in vec3 Position;
in vec3 Light_Color;
in float col;

void main()
{
	FragColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	FragColor.xyz = vec3(col, col, col);
	FragColor.xyz = FragColor.xyz * Light_Color;
	FragColor.w = 1.0f;
}
