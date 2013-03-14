#include "stdafx.h"

#include "SimulatorModule.h"

#include <iostream>
#include <random>
#include <time.h>
#include <math.h>

#define _USE_MATH_DEFINES

// Function forward declerations
void computeFAc( bool ***, bool ***, int, int, int, int, int, int );
float distFrom( int, int, int, int, int, int );

SimulatorModule::SimulatorModule() {
	
	pHumExt = 0.1f;
	pActExt = 0.001f;
	pCldExt = 0.1f;
	randomResolution = 10000;

	srand( time(NULL) );

}

void SimulatorModule::initialize() {
}

void SimulatorModule::terminate() {
}

void SimulatorModule::stepAsych( SimulationData* data ) {
	
	simulateCellular( data->x, data->y, data->z, data->hum, 
		data->act, data->cld, data->fAc );
	
	calculateDensity( data->x, data->y, data->z, data->cld, data->workDen );

}

void SimulatorModule::stepMutex( SimulationData* data, double time ) {
	
	// Copy nextDen to prevDen and workDen to nextDen
	copyGrid( data->prevDen, data->nextDen, data->x, data->y, data->z );
	copyGrid( data->nextDen, data->workDen, data->x, data->y, data->z );
	// Update times
	data->prevTime = data->nextTime;
	data->nextTime = time;

	//updateMetaballs( data->x, data->y, data->z, data->den, data->metaballs );

}

void SimulatorModule::simulateCellular( int x, int y, int z, bool *** hum, bool
									  *** act, bool *** cld, bool *** fAc ) {
	
	for( int i = 0; i != x; ++i )
		for( int j = 0; j != y; ++j )
			for(int k = 0; k != z; ++k )
				computeFAc( fAc, act, i, j, k, x, y, z );
	
	for( int i = 0; i != x; ++i )
		for( int j = 0; j != y; ++j )
			for(int k = 0; k != z; ++k) {

				// Compute clouds first, since they don't influence  act/hum
				cld[i][j][k] = cld[i][j][k] || act[i][j][k];
				bool newAct = !act[i][j][k] && hum[i][j][k] &&
					fAc[i][j][k];
				hum[i][j][k] = hum[i][j][k] && !act[i][j][k];
				act[i][j][k] = newAct;

				// Scale probabilities with the distance from nearest elipsoid
				float dist1 = distFrom(i, j, k, x/2, (y-5)/2, z/2);
				float dist2 = distFrom(i, j, k, (x+75)/2, (y)/2, (z-9)/2);
				float distance = dist1 < dist2 ? dist1 : dist2;

				float cloudSize = 20;

				// Extinction probability increases with distance, other two
				// decrease
				float scaledPCldExt = pCldExt * ( 1 + distance / cloudSize);
				if( scaledPCldExt > 1 ) scaledPCldExt = 1;
				float scaledPHumExt = pHumExt * ( 1 - distance / cloudSize);
				if( scaledPHumExt < 0 ) scaledPHumExt = 0;
				float scaledPActExt = pActExt * ( 1 - distance / cloudSize);
				if( scaledPActExt < 0 ) scaledPActExt = 0;

				// Cloud extinction
				if( cld[i][j][k] )
					if( (float)(rand() % randomResolution)/randomResolution < 
						scaledPCldExt )
						cld[i][j][k] = false;
				if( (float)(rand() % randomResolution)/randomResolution < 
					scaledPHumExt )
					hum[i][j][k] = true;
				if( (float)(rand() % randomResolution)/randomResolution < 
					scaledPActExt )
					act[i][j][k] = true;
								
			}

}

void SimulatorModule::calculateDensity( int x, int y, int z, 
									   bool *** cld, float *** den ) {

	int S = 5; // Blur matrix is size SxSxS
	for( int i = 0; i != x; ++i )
		for( int j = 0; j != y; ++j )
			for( int k = 0; k != z; ++k)
				// Do a box blur
				den[i][j][k] = singleDensity( x, y, z, i, j, k, cld, S);

}

float SimulatorModule::singleDensity( int x, int y, int z, int i, int j, int k,
									bool *** cld, int S ) {
	// Go through kernel
	int halfS = (S-1)/2;
	float sum = 0;
	for( int kI = i-halfS; kI <= i+halfS; ++kI )
		for( int kJ = j-halfS; kJ <= j+halfS; ++kJ )
			for( int kK = k-halfS; kK <= k+halfS; ++kK ){
				// Check if kernel isn't in the grid
				if( kI < 0 || kJ < 0 || kK < 0 ||
					kI >= x || kJ >= y || kK >= z )
					continue; // Skip if it is
				if( cld[kI][kJ][kK] ) sum += 1; // w_i=1; box filter
			}
	return sum/(S*S*S);
}

float SimulatorModule::fieldFunction( float a ) {

	return -4.0f/9.0f * pow(a,6) + 17.0f/9 * pow(a,4) - 22.0f/9 * pow(a,2) + 1;

}

void SimulatorModule::copyGrid( float*** copyTo, float*** copyFrom, int x, int y, int z ) {
		
	for( int i = 0; i != x; ++i )
		for( int j = 0; j != y; ++j )
			for(int k = 0; k != z; ++k) 
				copyTo[i][j][k] = copyFrom[i][j][k];

}


// Computes f_act(i,j,k) function. fAc is size x*y*z 
void computeFAc( bool *** fAc, bool *** act, int i, int j, int k, 
				int x, int y, int z ) {

	if( i+1 < x && act[i+1][j][k] ||
		j+1 < y && act[i][j+1][k] ||
		k+1 < z && act[i][j][k+1] ||
		i-1 >= 0 && act[i-1][j][k] ||
		j-1 >= 0 && act[i][j-1][k] ||
		k-1 >= 0 && act[i][j][k-1] ||

		i+2 < x && act[i+2][j][k] ||
		j+2 < y && act[i][j+2][k] ||
		i-2 >= 0 && act[i-2][j][k] ||
		j-2 >= 0 && act[i][j-2][k] ||
		k-2 >= 0 && act[i][j][k-2] )
		fAc[i][j][k] = true;
	else fAc[i][j][k] = false;

}

// Compute the distance of point x1 y1 z1 from point x2 y2 z2 (second norm)
float distFrom( int x1, int y1, int z1, int x2, int y2, int z2 ) {

	return static_cast<float>(sqrt( 
		(x1-x2)*(x1-x2) + (y1-y2)*(y1-y2) + (z1-z2)*(z1-z2) ));

}
