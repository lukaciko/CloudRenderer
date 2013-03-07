#version 150

in vec3 color;

uniform vec3 viewDirection;

out vec4 outColor;

void main() {
	outColor = vec4( color, 1.0 );
}