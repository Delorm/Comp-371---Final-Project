#version 330 core
  
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 uv_coord;
layout (location = 2) in vec3 normal;

out vec2 tex_coord;
out vec3 pix_normal;
out vec3 light_dir;
out vec3 view_vector;

uniform mat4 mvp_matrix;
uniform mat4 m_matrix;
uniform vec4 light_direction;
uniform vec4 eye_location;

void main()
{
    gl_Position =  mvp_matrix * vec4(position.x, position.y, position.z, 1.0);
    vec4 normal4d =  m_matrix * vec4(normal, 0.0);
    vec4 light4d = light_direction;

    light_dir = normalize(light4d.xyz);

    view_vector = (eye_location - (m_matrix * vec4(position, 1.0))).xyz;
    view_vector = normalize(view_vector);

    tex_coord = uv_coord;
}

