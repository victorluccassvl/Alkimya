#version 330 core

layout ( location = 0 ) in vec3 attrib_pos;
layout ( location = 1 ) in vec2 attrib_cords;

out vec2 cords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4( attrib_pos, 1.0f );
	cords = vec2( attrib_cords.x, 1 - attrib_cords.y );
}