#version 450 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;
layout (location = 2) in vec2 aTexCoord;

out vec3 Light_Color;
out vec3 col;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 color; 

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0f);
	float intensity = dot(aNorm, normalize(vec3(0.5f, 1.0f, 0.5))) * 0.5f
		+ 0.5f;
	Light_Color = vec3(1.0f, 1.0f, 1.0f) * 0.5f
		+ vec3(intensity, intensity, intensity) * 0.5f;
	col = color;
}
