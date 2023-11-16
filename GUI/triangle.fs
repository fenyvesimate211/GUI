#version 410

in vec3 vPos;
out vec4 frag_colour;
uniform vec3 color;
uniform vec3 selectedPoint;
uniform vec3 selectedColor;

void main()
{
	frag_colour = vec4(color, 1.0);
	if(vPos == selectedPoint)
		frag_colour = vec4(selectedColor, 1.0);
}