#version 330 core
 
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec3 normal;

out vec2 tex_coord;
out vec3 pix_normal;
out vec3 light_dir;
out vec3 view_vector;


uniform mat4 mvp_matrix;
uniform mat4 m_matrix;
uniform mat4 v_matrix;
uniform vec4 light_direction;
uniform vec4 eye_location;
uniform vec4 clipping_plane;

out float visibility;
uniform float density;
uniform float gradient;

void main()
{

    
    gl_ClipDistance[0] = dot(m_matrix * vec4(position, 1), clipping_plane);
    gl_Position =  mvp_matrix * vec4(position, 1.0);
    vec4 normal4d =  m_matrix * vec4(normal, 0.0);
    vec4 light4d = light_direction;

    pix_normal = normalize(normal4d.xyz);
    light_dir = normalize(light4d.xyz);

    view_vector = (eye_location - (m_matrix * vec4(position, 1.0))).xyz;
    view_vector = normalize(view_vector);

    tex_coord = texCoord;

   //Fog
    vec4 positionRelativeToCam = v_matrix * m_matrix * vec4(position, 1);
    float distance = length(positionRelativeToCam.xyz);
    visibility = exp(-pow((distance*density), gradient));
    visibility = clamp(visibility,0.0,1.0);    

}
