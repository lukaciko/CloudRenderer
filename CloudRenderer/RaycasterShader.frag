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

uniform float densityCutoff = 0.06f;
uniform float densityFactor = 0.35f;
uniform float attenuationFactor = 0.15F;
uniform float colorMultiplier = 5.0f;
uniform float lightColorRed = 1.0f;
uniform float lightColorGreen = 1.0f;
uniform float lightColorBlue = 1.0f;
uniform float shadeColorRed = 0.0f;
uniform float shadeColorGreen = 0.0f;
uniform float shadeColorBlue = 0.2f;
uniform float sunPositionX = 0.0f;
uniform float sunPositionY = 0.5f;
uniform float sunPositionZ = 0.5f;

uniform float viewSamplesF = 128.0f;
uniform float lightSamplesF = 64.0f;

int viewSamples = int(viewSamplesF);
int lightSamples = int(lightSamplesF);

const float maxDistance = sqrt(3.0); // Length of a cube diagonal
float lightStepSize = maxDistance/viewSamples;

out vec4 outColor;

struct Ray {
    vec3 origin;
    vec3 direction; // Normalized
};

// Perform slab method for ray/box intersection. Box spans (-1,-1,-1), (1,1,1)
// Returns true if there is an intersection
// t0, t1 - distances to the two intersections
bool IntersectRayBox(Ray r, out float t0, out float t1) {
    vec3 invR = 1.0 / r.direction;
    vec3 tbot = invR * (vec3( -1, -1, -1), - r.origin);
    vec3 ttop = invR * (vec3( 1, 1, 1) - r.origin);
    vec3 tmin = min(ttop, tbot);
    vec3 tmax = max(ttop, tbot);
    vec2 t = max(tmin.xx, tmin.yz);
    t0 = max(t.x, t.y);
    t = min(tmax.xx, tmax.yz);
    t1 = min(t.x, t.y);
    return t0 <= t1;
}

void main() {
	
	// Direction in view splace
	vec3 viewDirection;
	viewDirection.xy = 2.0f * gl_FragCoord.xy / screenSize - 1.0f;
	viewDirection.z = -1.0f / tanFOV;
	// FOV corresponds to Y axis, scale x accordingly
	viewDirection.x *= screenSize.x / screenSize.y;

	// Transform direction to world	space
	viewDirection = ( viewInverse * vec4( viewDirection, 0 ) ).xyz;
	
	Ray viewRay = Ray( eyePosition, normalize( viewDirection ) );	

	vec3 color = vec3( 67/256.0, 128/256.0, 183/256.0 );
	vec3 pos = viewRay.origin;
	float tmin, tmax;
	IntersectRayBox( viewRay, tmin, tmax );
	pos += tmax * viewRay.direction;
	float viewStepSize = ( tmax - tmin ) / viewSamples;

	vec3 shadeColor = vec3( shadeColorRed, shadeColorGreen, shadeColorBlue );
	vec3 lightColor = vec3( lightColorRed, lightColorGreen, lightColorBlue );
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
				// Closer texture reads contribute more
				attenuation *= 1 - 
					texture( density, lightPos ).x	
					* attenuationFactor				 
					* ( 1 - j / lightSamples );
				lightPos += lightRay.direction * lightStepSize;
			}

			// Add color depending on cell density and attenuation
			if( cellDensity > 0.001 ) {
				color = mix( color, mix ( shadeColor, lightColor, attenuation ), 
			         cellDensity * colorMultiplier);
			}
		}

		pos -= viewRay.direction * viewStepSize;

	}

	outColor = vec4( color, 255 );
	vec4 debug = vec4( viewRay.direction, 1.0 );
	outColor = mix( outColor, debug, 0.0 );
}