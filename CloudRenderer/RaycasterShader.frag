#version 330

uniform sampler3D density;

// tan( field of view / 2 )
uniform float tanFOV; 
// Camera position in world space
uniform vec3 eyePosition;
uniform vec2 screenSize;
uniform mat4 viewInverse;

float near;
float far;

uniform float densityCutoff = 0.06;
uniform float densityFactor = 0.35;
uniform float attenuationFactor = 0.05;
uniform float colorMultiplier = 5.0f;
uniform float shadeColorRed = 0.0f;
uniform float shadeColorGreen = 0.0f;
uniform float shadeColorBlue = 0.2f;
uniform float sunPositionX = 0.5f;
uniform float sunPositionY = 0.5f;
uniform float sunPositionZ = 0.5f;

const float maxDistance = sqrt(3.0); // Length of a cube diagonal
const int viewSamples = 128;
const float viewStepSize = maxDistance/viewSamples;
const int lightSamples = 64;
const float lightStepSize = maxDistance/viewSamples;

out vec4 outColor;

struct Ray {
    vec3 origin;
    vec3 direction; // Normalized
};

void main() {
	
	// Direction in view splace
	vec3 viewDirection;
	viewDirection.xy = 2.0f * gl_FragCoord.xy / screenSize - 1.0f;
	//viewDirection.xy *= tanFOV; // tan( fov / 2 )
	viewDirection.z = -1/tanFOV; //-1;

	// Transform direction to world	space
	viewDirection = ( viewInverse * vec4( viewDirection, 0 ) ).xyz;
	
	Ray viewRay = Ray( eyePosition, normalize( viewDirection ) );	

	vec3 color = vec3( 155/256.0, 225/256.0, 251/256.0 );
	vec3 pos = viewRay.origin;
	pos += viewDirection * viewStepSize * viewSamples;
	//TODO: move to cube beginning

	vec3 shadeColor = vec3( shadeColorRed, shadeColorGreen, shadeColorBlue );
	vec3 sunPosition = vec3( sunPositionX, sunPositionY, sunPositionZ );

	for( int i = 0; i < viewSamples; ++i ) {
		
		float cellDensity = texture( density, pos ).x;
		if( cellDensity > densityCutoff ) {
			
			cellDensity *= densityFactor;
		
			Ray lightRay = Ray( pos, normalize( sunPosition ) );

			float attenuation = 1;
			vec3 lightPos = pos;
			// Calculate light attenuation
			for( int j = 0; j < lightSamples; ++j ) {

				attenuation *= 1 - 
					texture( density, lightPos ).x	
					* attenuationFactor				 
					* ( 1 - j / lightSamples );		// Closer texture reads contribute more
				// TODO: optimize - check for break
				lightPos += lightRay.direction * lightStepSize;

			}

			// add color depending on cell density and attenuation
			if( cellDensity > 0.001 ) {
				color = mix( color, vec3( mix ( attenuation, 1.0, shadeColor.x ), 
				                          mix ( attenuation, 1.0, shadeColor.y ), 
										  mix ( attenuation, 1.0, shadeColor.z )), 
					         cellDensity * colorMultiplier);
			}
		}

		pos -= viewRay.direction * viewStepSize;

	}

	outColor = vec4( color, 255 );
	vec4 debug = vec4( viewRay.direction, 1.0 );
	outColor = mix( outColor, debug, 0.00 );
}