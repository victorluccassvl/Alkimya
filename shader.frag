#version 330 core

out vec4 frag_color;

in vec3 color;
in vec2 text;

uniform sampler2D my_texture;

void main()
{
	frag_color = texture( my_texture, text ) * vec4( color, 1.0f );
}