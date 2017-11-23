#version 330 core

in vec2 tex_coord;
out vec4 color;

uniform sampler2D day;
uniform sampler2D night;
uniform vec4 light_direction;



void main()
{
	
	float dim = 0.7;
	vec3 day_col = texture(day, tex_coord).xyz;
	vec3 night_col = texture(night, tex_coord).xyz;
	vec3 light = light_direction.xyz;
	
	// Blending Factor
	float f = (dot(light, vec3(0, 1, 0)) + 1) / 2.0f;
	vec3 blend = (1.0 - f) * day_col + f * night_col;
	blend *= dim;
	color = vec4(blend, 1.0);
}
