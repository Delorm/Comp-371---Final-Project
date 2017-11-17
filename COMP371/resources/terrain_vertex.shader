#version 330 core
  
layout (location = 0) in vec3 position;

out vec2 tex_coord;

uniform mat4 mvp_matrix;
uniform mat4 m_matrix;

void main()
{
    gl_Position =  mvp_matrix * vec4(position.x, position.y, position.z, 1.0);

    vec4 model_position = m_matrix * vec4(position.x, position.y, position.z, 1.0); 
    float tiling = 4.0;
    tex_coord = vec2(model_position.x / tiling, 1.0f - model_position.z / tiling);
}

