#version 330 core

in vec2 tex_coord;

out vec4 color;

uniform sampler2D background;
uniform sampler2D r_texture;
uniform sampler2D g_texture;
uniform sampler2D b_texture;
uniform sampler2D blend_map;

void main()
{
    vec4 blend_color = texture(blend_map, tex_coord); 
    vec2 tiled_coord = tex_coord * 50.0;

    float back_amount = 1.0f - (blend_color.r + blend_color.g + blend_color.b);
    vec4 back_color = back_amount * texture(background, tiled_coord);
    vec4 r_color = texture(r_texture, tiled_coord) * blend_color.r;
    vec4 g_color = texture(g_texture, tiled_coord) * blend_color.g;
    vec4 b_color = texture(b_texture, tiled_coord) * blend_color.b;
    vec4 total_color = back_color + r_color + g_color + b_color;


    color = total_color;
} 
