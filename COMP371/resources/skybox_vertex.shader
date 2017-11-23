#version 330 core
 
layout (location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord;

uniform mat4 mvp_matrix;

out vec2 tex_coord;

void main()
{
	gl_Position = mvp_matrix * vec4(position, 1.0);
	tex_coord = vec2(texCoord.x, texCoord.y);
}
