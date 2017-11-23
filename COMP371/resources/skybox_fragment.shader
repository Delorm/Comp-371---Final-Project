#version 330 core

in vec2 tex_coord;
out vec4 color;

uniform sampler2D ourTexture1;


void main()
{
	vec4 myTex = texture(ourTexture1, tex_coord);
	if (myTex.a < 0.5) discard;
	color = myTex;
}
