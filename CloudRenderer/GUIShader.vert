#version 150

in vec2 position;
in vec2 texCoord;

out vec2 TexCoord;

uniform float offsetX;
uniform float offsetY;

void main() {
	TexCoord = texCoord;
	// Render them in upper right corner
	gl_Position = vec4( position.x + offsetX, position.y + offsetY, 0.0, 1.0 );
}