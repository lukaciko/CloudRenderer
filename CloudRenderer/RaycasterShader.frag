#version 330

in vec3 color;

uniform sampler3D density;

// tan( field of view / 2 )
uniform float tanFOV; 
// Camera position in world space
uniform vec3 eyePosition;
uniform vec2 screenSize;
uniform mat4 viewInverse;

uniform float near;
uniform float far;

const int numSamples = 300;
const float maxDistance = sqrt(3.0); // Length of a cube diagonal
const float stepSize = maxDistance/numSamples;
const float densityFactor = 0.15;

out vec4 outColor;

void main() {

	vec3 pos = eyePosition; // World space

	vec3 colorSum = vec3(0.0);
	
	// Direction in view splace
	vec3 direction;
	direction.xy = 2.0f * gl_FragCoord.xy / screenSize - 1.0f;
	direction.xy *= tanFOV; // tan(fov/2)
	direction.z = -1;

	// Transform direction to world space
	direction = ( viewInverse * vec4( direction, 0 ) ).xyz;
	direction = normalize( direction );	

	for( int i = 0; i < numSamples; ++i ) {
		
		float cellDensity = texture( density, pos );
		cellDensity *= densityFactor;
		colorSum += vec3( cellDensity, cellDensity, cellDensity );

		pos += direction * stepSize;

	}

	outColor = vec4( 1.0, 1.0, 1.0, colorSum.x );
	vec4 debug = vec4( direction, 1.0 );
	outColor = mix(outColor, debug, 0);
}