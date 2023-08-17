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
//	if (Position.y > -0.1f && Position.y < 0.1f) {
//		if (Position.x < 4.0f && Position.x > -4.0f) {
//			FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
//		} else {
//			FragColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);
//		}
//	} else {
//		FragColor = vec4(0.1f, 0.1f, 0.1f, 1.0f);
//	}

	FragColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	FragColor = texture(texture_diffuse1, TexCoords);
	FragColor.w = 1.0f;
//	FragColor = vec4(TexCoords.y, TexCoords.y, TexCoords.y, 1.0f); 
//	FragColor.xyz = Light_Color * 0.3f + Board_Color * 0.7f;
//	FragColor.xyz = vec3(0.5f, 0.5f, 0.5f) + 0.5f * color;
//	FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
}
