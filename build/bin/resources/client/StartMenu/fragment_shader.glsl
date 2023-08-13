#version 450 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 color;

uniform sampler2D texture_diffuse1;

void main()
{    
//	FragColor = texture(texture_diffuse1, TexCoords);
	FragColor.xyz = vec3(0.5f, 0.5f, 0.5f) + 0.5f * color;
	FragColor.w = 1.0f;
//	FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
}
