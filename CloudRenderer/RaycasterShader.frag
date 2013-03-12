#version 330

in vec3 color;

uniform sampler3D density;
uniform vec3 viewDirection;

const float moveFactor = 0.05;

out vec4 outColor;

void main() {

	// Start at the color of the cube

	vec3 pos = color;
   	
	vec3 colorSum = vec3(0.0);
	pos = pos.zyx;

	vec3 direction = viewDirection.zyx;
	direction = vec3(direction.x, -direction.y, -direction.z);

	for( int i = 0; i < 250; ++i ) {
		
		float cellDensity = texture( density, pos );
		cellDensity *= 0.974;

		colorSum += vec3( cellDensity, cellDensity, cellDensity );

		pos += direction * moveFactor;

	}

	float l = length(color);
	l /=2;

	outColor = 0.2*vec4(color, 1.0) + 0.8*vec4( colorSum.x, 0.0, 0.0, 1.0 );
}