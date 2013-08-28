#version 150

in vec3 cubeVert;

uniform mat4 view;
uniform mat4 proj;

void main() {
	gl_Position = proj * view * vec4(cubeVert, 1.0f);
}