#version 330 core

out vec4 frag_color;

in vec2 cords;

uniform sampler2D text1;
uniform sampler2D text2;

void main()
{
	vec2 cords2 = vec2( 1 - cords.x, cords.y );
	frag_color = mix( texture( text1, cords ), texture( text2, cords2 ), 0.2f );
	frag_color = frag_color;
}