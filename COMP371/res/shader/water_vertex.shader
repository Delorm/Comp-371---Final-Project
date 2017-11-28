#version 330 core
  
layout (location = 0) in vec3 position;

out vec4 position4d;
out vec3 view_vector;
out vec2 tex_coord;

uniform mat4 mvp_matrix;
uniform mat4 m_matrix;
uniform vec4 eye_location;

void main()
{
    gl_Position =  mvp_matrix * vec4(position.x, position.y, position.z, 1.0);
    position4d = gl_Position;

    view_vector = (eye_location - (m_matrix * vec4(position, 1.0))).xyz;

    float tiling = 16.0;
    tex_coord = vec2(position.x, position.z) * tiling;
    
}

