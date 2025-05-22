#version 460 core

layout(location = 0) in vec2 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_TexCoords;
layout(location = 3) in int a_TexIndex;

out vec4 v_Color;
out vec2 v_TexCoords;
out int v_TexIndex;

void main()
{
	v_TexCoords = a_TexCoords;
	v_TexIndex = a_TexIndex;
	v_Color = a_Color;
	gl_Position = vec4(a_Position, 0.0f, 1.0);
}