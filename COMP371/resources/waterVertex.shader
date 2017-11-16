#version 330 core

layout(location = 0) in vec3 position;
out vec3 colors;
out vec2 coords;

uniform mat4 mvp_matrix;

void main()
{
	gl_Position = mvp_matrix * vec4(position.x, position.y, position.z, 1.0);
	coords = vec2(position.x/2.0 + 0.5,position.y/2.0 + 0.5);

	
}

