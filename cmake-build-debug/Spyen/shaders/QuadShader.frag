#version 460 core

in vec4 v_Color;
in vec2 v_TexCoords;
in flat int v_TexIndex;

uniform sampler2D u_Textures[32];

out vec4 FragColor;

void main()
{
	//FragColor = vec4(v_TexIndex / 1.0, 0.0, 0.0, 1.0); // red = index / 32
	FragColor = texture(u_Textures[v_TexIndex], v_TexCoords) * v_Color;
}