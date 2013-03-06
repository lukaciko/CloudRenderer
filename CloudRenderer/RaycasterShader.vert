#version 150

in vec3 cubeVert;
in vec3 startColor;

uniform vec3 position;
uniform mat4 view;
uniform mat4 proj;

out vec3 color;

void main() {
	gl_Position = proj * view * vec4(cubeVert,1.0f);
	color = startColor;
}