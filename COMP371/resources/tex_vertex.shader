#version 330 core
 
layout (location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord;

uniform mat4 mvp_matrix;

out vec2 TexCoord;

void main()
{
	gl_Position = mvp_matrix * vec4(position, 1.0);
    //gl_Position = projection * view * model * vec4(position.x, position.y, position.z, 1.0f);
	//gl_Position = vec4(position, 1.0f);

	//gl_PositionStatic = vec4(position, 1.0);

	TexCoord = vec2(texCoord.x, 1.0 - texCoord.y);
}
