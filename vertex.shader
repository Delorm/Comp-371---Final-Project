#version 330 core
  
layout (location = 0) in vec3 position;
layout (location = 2) in vec3 color;
uniform int object_type;
out vec3 colors;

uniform mat4 mvp_matrix;

void main()
{
    gl_Position =  mvp_matrix * vec4(position.x, position.y, position.z, 1.0);
    colors = color;
}

