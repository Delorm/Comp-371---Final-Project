#version 330 core
  
layout (location = 0) in vec3 position;
out vec3 colors;

uniform mat4 mvp_matrix;

void main()
{
    gl_Position =  mvp_matrix * vec4(position, 1.0);
    colors = vec3(1, 0, 0);
}

