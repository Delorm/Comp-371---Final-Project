#version 330 core
 
layout (location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord;

uniform mat4 mvp_matrix;
uniform mat4 m_matrix;

out vec2 tex_coord;
out float height;

void main()
{
	gl_Position = mvp_matrix * vec4(position, 1.0);
	height = (position.y + 1) / 2.0f;
	tex_coord = vec2(texCoord.x, texCoord.y);
}
