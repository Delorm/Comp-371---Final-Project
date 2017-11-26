#version 330 core

in vec2 tex_coord;
in vec3 light_dir;
in vec3 pix_normal;
in vec3 view_vector;

out vec4 color;
uniform sampler2D ourTexture1;

in float visibility;
uniform vec3 skyColor;


void main()
{
    vec4 myTex = texture(ourTexture1, tex_coord);
    if (myTex.a < 0.5) discard;

    vec4 object_color = myTex;

    float color_strength = 0.0f;

    // Ambience
    float ka = 0.2;
    color_strength += ka;

    // Diffuse
    float strength = max(dot(-light_dir, pix_normal), 0);
    float kd = 0.6f;
    color_strength +=  kd * strength;

    // Specular
    float a = 10;
    float ks = 0.2;
    vec3 reflection_vector = reflect(-light_dir, pix_normal);
    float prod = max(dot(view_vector, reflection_vector) , 0);
    strength = pow(prod, a);
    color_strength += ks * strength;

    color = color_strength * object_color;

    //color = mix(vec4(skyColor, 1.0f), color, visibility);

}
