#version 450 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;
layout (location = 2) in vec2 aTexCoord;

out vec2 TexCoords;
out vec3 color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0);
	float intensity = dot(aNorm, normalize(vec3(1.0, 1.0, 1.0)));
	//color = aNorm * vec3(1.0, 1.0, 1.0);
	color = vec3(intensity, intensity, intensity);
	TexCoords = aTexCoord;
}
