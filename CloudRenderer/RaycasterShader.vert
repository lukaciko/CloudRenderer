#version 150

in vec3 cubeVert;

uniform mat4 model;
uniform vec3 position;
uniform mat4 view;
uniform mat4 proj;

void main() {
	gl_Position = vec4(cubeVert,1.0f);
}