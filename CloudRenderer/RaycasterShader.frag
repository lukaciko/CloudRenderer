#version 330

in vec3 color;

uniform mat4 view;
uniform sampler3D density;
uniform float focalLength;
uniform vec2 screenSize;
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
	//pos = pos.zyx;

	vec3 direction;
	direction.xy = 2.0f * gl_FragCoord.xy / screenSize - 1.0f;
	direction.z = - focalLength;
	//direction = ( vec4( direction, 0 ) * view ).xyz;


	//direction = direction.zyx;
	//direction = vec3(direction.x, -direction.y, -direction.z);

	for( int i = 0; i < numSamples; ++i ) {
		
		float cellDensity = texture( density, pos );
		cellDensity *= densityFactor;

		colorSum += vec3( cellDensity, cellDensity, cellDensity );

		pos += direction * stepSize;

	}

	outColor = vec4( 1.0, 0.0, 1.0, colorSum.x );
	vec4 debug = vec4( direction, 1.0 );
	outColor = mix(outColor, debug, 0.6);
}