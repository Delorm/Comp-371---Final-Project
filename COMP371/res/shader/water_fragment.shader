#version 330 core

out vec4 color;
in vec4 position4d;
in vec2 tex_coord;
in vec3 view_vector;

uniform sampler2D reflection;
uniform sampler2D refraction;
uniform sampler2D dudv;
uniform float offset;

void main()
{
    vec2 screen_coord = ((position4d.xy / position4d.w) + 1) * 0.5;

    float mult = 0.02;
    vec2 dist = mult * texture(dudv, vec2(tex_coord.x + offset, tex_coord.y)).xy;
    screen_coord += dist;
    screen_coord = clamp(screen_coord, 0, 1);

    vec4 reflection_color = texture(reflection, vec2(screen_coord.x, 1-screen_coord.y));
    vec4 refraction_color = texture(refraction, screen_coord);
    
    float f = dot(normalize(view_vector), vec3(0, 1, 0));
    color = mix(reflection_color, refraction_color, f);
    color += vec4(0, 0, 0.05, 1);
} 
