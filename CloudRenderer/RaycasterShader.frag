#version 330

in vec3 color;

uniform mat4 view;
uniform mat4 viewInverse;
uniform mat4 proj;
uniform mat4 projInverse;
uniform sampler3D density;
uniform float focalLength;
uniform vec2 screenSize;
uniform vec3 viewDirection;
uniform vec3 eyePosition;

const int numSamples = 300;
const float maxDistance = sqrt(2.0);
const float stepSize = maxDistance/numSamples;
const float densityFactor = 0.15;

out vec4 outColor;

void main() {

	vec3 pos = eyePosition;
	//pos = vec3(0);

	vec3 colorSum = vec3(0.0);

	vec3 direction;
	direction.xy = 2.0f * gl_FragCoord.xy / screenSize - 1.0f;
	direction.z = - focalLength;
	direction = ( (viewInverse * projInverse) * vec4( direction, 0 ) ).xyz;
	direction = normalize( direction );	

	for( int i = 0; i < numSamples; ++i ) {
		
		float cellDensity = texture( density, pos );
		cellDensity *= densityFactor;

		colorSum += vec3( cellDensity, cellDensity, cellDensity );

		pos += direction * stepSize;

	}

	outColor = vec4( 1.0, 0.0, 1.0, colorSum.x );
	vec4 debug = vec4( direction, 1.0 );
	outColor = mix(outColor, debug, 0.5);
}