#version 330 core

layout ( location = 0 ) in vec3 attrib_position;
layout ( location = 1 ) in vec3 attrib_normal;
layout ( location = 2 ) in vec2 attrib_texture_coords;

out vec2 texture_coords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	texture_coords = attrib_texture_coords;
	gl_Position = projection * view * model * vec4( attrib_position, 1.0f );
}