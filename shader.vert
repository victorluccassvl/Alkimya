#version 330 core

layout ( location = 0 ) in vec3 attrib_pos;
layout ( location = 1 ) in vec3 attrib_color;
layout ( location = 2 ) in vec2 attrib_text;

out vec3 color;
out vec2 text;

void main()
{
	gl_Position = vec4( attrib_pos.x, attrib_pos.y, attrib_pos.z, 1.0f );
	color = attrib_color;
	text = attrib_text;
}