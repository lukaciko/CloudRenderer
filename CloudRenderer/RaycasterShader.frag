#version 330

in vec3 color;

uniform sampler3D density;
uniform vec3 viewDirection;

const int numSamples = 100;
const float maxDistance = sqrt(2.0);
const float stepSize = maxDistance/numSamples;
const float densityFactor = 0.15;

out vec4 outColor;

void main() {

	// Start at the color of the cube

	vec3 pos = color;
   	
	vec3 colorSum = vec3(0.0);
	pos = pos.zyx;

	vec3 direction = viewDirection.zyx;
	direction = vec3(direction.x, -direction.y, -direction.z);

	for( int i = 0; i < numSamples; ++i ) {
		
		float cellDensity = texture( density, pos );
		cellDensity *= densityFactor;

		colorSum += vec3( cellDensity, cellDensity, cellDensity );

		pos += direction * stepSize;

	}

	outColor = vec4( 1.0, 0.0, 1.0, colorSum.x );
}