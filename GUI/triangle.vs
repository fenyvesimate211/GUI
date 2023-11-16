#version 410

in vec3 vp;
out vec3 vPos;
uniform mat4 MVP;

void main()
{
	gl_Position = MVP * vec4(vp, 1.0);
	vPos = vp;
}