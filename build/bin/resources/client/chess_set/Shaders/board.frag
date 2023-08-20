#version 450 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 Position;
in vec3 Light_Color;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_base_color1;

void main()
{    
	FragColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	FragColor = texture(texture_diffuse1, TexCoords);
	FragColor.xyz = FragColor.xyz * Light_Color;
	FragColor.w = 1.0f;
}
