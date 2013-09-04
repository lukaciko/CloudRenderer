#version 150

in vec2 coord;

out vec2 Coord;

uniform float sizeX;
uniform float sizeY;
uniform float positionX;
uniform float positionY;


void main() {
	Coord = coord;
	// Render them in upper right corner
	gl_Position = vec4( positionX + ( coord.x * 2 - 1 ) * sizeX, 
	   positionY + ( coord.y * 2 - 1) * sizeY, 0.0, 1.0 );
}