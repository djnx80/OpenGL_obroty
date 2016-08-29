#version 450

layout(location = 0) in vec3 pozycja;
layout(location = 1) in vec3 kolor_punktu;

uniform mat4 macierz_przesuniecia;

out vec3 kolor;

void main()
{
	kolor = kolor_punktu;
	gl_Position = macierz_przesuniecia*vec4(pozycja, 1.0);
}