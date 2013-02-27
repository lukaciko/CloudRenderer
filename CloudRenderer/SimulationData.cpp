#include "stdafx.h"

#include "SimulationData.h"

#include <math.h>
#include <random>
#include <time.h>

SimulationData::SimulationData(int gridLength, int gridWidth, int gridHeight, 
	float R ) : x(gridLength), y(gridWidth), z(gridHeight), metaR(R) { 

	// Allocation of grids
	hum = new bool ** [gridLength];
	cld = new bool ** [gridLength];
	act = new bool ** [gridLength];
	fAc = new bool ** [gridLength];
	workDen = new float ** [gridLength];
	prevDen = new float ** [gridLength];
	nextDen = new float ** [gridLength];

	make3Darray( hum, x, y, z );
	make3Darray( cld, x, y, z );
	make3Darray( act, x, y, z );
	make3Darray( fAc, x, y, z );
	make3Darray( workDen, x, y, z );
	make3Darray( prevDen, x, y, z );
	make3Darray( nextDen, x, y, z );

	srand( time(NULL) );
	prevTime = 0; // TODO: set to glfw time
	nextTime = 0;

	// hum and act set randomly, cld 0
	for( int i = 0; i != gridLength; ++i )
		for( int j = 0; j != gridWidth; ++j )
			for(int k = 0; k != gridHeight; ++k ) {
				if( rand() % 5 == 0 )
					hum[i][j][k] = true;
				if( rand() % 1500 == 0 )
					act[i][j][k] = true;
			}

	// Metaball initialization
	// Metaballs must cover all the area represented by the grid. It's easiest to
	// treat them as regulal tetrahedrons - metaball is the circumsphere of a 
	// tetrahedron and tetrahedrons form a triangulation of the area.

	// The size of tetrahedrons edge
	metaA = static_cast<float>( sqrt( 8.0/3.0 ) ) * metaR;
	// The height of tetrahedrons face
	metaV = static_cast<float>( sqrt( 2.0 ) ) * metaR;

	// Number of metaballs in each direction
	int metaX = ceil(x/metaV);
	int metaY = ceil(y/metaV);
	int metaZ = ceil(z/metaV);

	for( int i = 0; i != metaX; ++i )
		for( int j = 0; j != metaY; ++j )
			for( int k = 0; k != metaZ; ++k ) {
				glm::vec3 metaPos = glm::vec3( i*metaV, j*metaV, k*metaV );
				metaballs.push_back( new Metaball( metaPos ) );
			}


}

SimulationData::~SimulationData() {

	// Deallocate the array memory and delete the pointers
	delete3Darray( hum, x, y, z );
	delete3Darray( cld, x, y, z );
	delete3Darray( act, x, y, z );
	delete3Darray( fAc, x, y, z );
	delete3Darray( workDen, x, y, z );
	delete3Darray( prevDen, x, y, z );
	delete3Darray( nextDen, x, y, z );
	delete hum;
	delete cld;
	delete act;
	delete fAc;
	delete workDen;
	delete prevDen;
	delete nextDen;

	for( metaVector::iterator i = metaballs.begin(); i != metaballs.end(); ++i )
		delete * i;

}


template <typename T>
void SimulationData::make3Darray( T *** arr, int x, int y, int z ) {

	// Allocation of a 3d array
	for( int i = 0; i != x; ++i ) {
		arr[i] = new T*[y];
		for (int j = 0; j != y; ++j ) 
			arr[i][j] = new T[z];
	}

	// Initialization of a 3d array
	for( int i = 0; i != x; ++i )
		for( int j = 0; j != y; ++j )
			for( int k = 0; k != z; ++k )
				arr[i][j][k] = 0;
}

template <typename T>
void SimulationData::delete3Darray( T *** arr, int x, int y, int z ) {

	// Deallocation of a 3d array
	for( int i = 0; i != x; ++i ) {
		for (int j = 0; j != y; ++j ) 
			delete [] arr[i][j];
		delete [] arr[i];
	}

}