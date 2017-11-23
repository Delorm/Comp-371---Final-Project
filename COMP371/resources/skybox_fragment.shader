#version 330 core

in vec2 tex_coord;
out vec4 color;

uniform sampler2D day;
uniform sampler2D night;
uniform vec4 light_direction;



void main()
{
	vec4 day_col = texture(day, tex_coord);
	vec4 night_col = texture(night, tex_coord);
	
	// Blending Factor
	float f = (dot(light_direction, vec4(0, 1, 0, 0)) + 1) / 2.0f;
	color = (1.0 - f) * day_col + f * night_col;
}
