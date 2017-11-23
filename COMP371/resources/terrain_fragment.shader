#version 330 core

in vec2 tex_coord;
in vec3 light_dir;
in vec3 pix_normal;
in vec3 view_vector;

out vec4 color;

uniform sampler2D background;
uniform sampler2D r_texture;
uniform sampler2D g_texture;
uniform sampler2D b_texture;
uniform sampler2D blend_map;
uniform sampler2D nor_map;

void main()
{
    vec3 normal = normalize(texture(nor_map, tex_coord).xyz);
    vec4 blend_color = texture(blend_map, tex_coord); 
    vec2 tiled_coord = tex_coord * 50.0;

    float back_amount = 1.0f - (blend_color.r + blend_color.g + blend_color.b);
    vec4 back_color = back_amount * texture(background, tiled_coord);
    vec4 r_color = texture(r_texture, tiled_coord) * blend_color.r;
    vec4 g_color = texture(g_texture, tiled_coord) * blend_color.g;
    vec4 b_color = texture(b_texture, tiled_coord) * blend_color.b;
    vec4 object_color = back_color + r_color + g_color + b_color;

    float color_strength = 0.0f;

    // Ambience
    float ka = 0.2;
    color_strength += ka;

    // Diffuse
    float diffuse_strength = max(dot(-light_dir, normal), 0);
    float kd = 0.2f;
    color_strength +=  (kd * diffuse_strength);

    // Specular

    float a = 20.0;
    float ks = 0.6;
    vec3 reflection_vector = reflect(-light_dir, normal);
    float prod = max(dot(view_vector, reflection_vector) , 0);
    float specular_strength = pow(prod, a);  
    color_strength += (ks * specular_strength);

    color = vec4(color_strength * object_color.xyz, 1);
} 
