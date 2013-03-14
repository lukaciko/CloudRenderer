#include "stdafx.h"

#include "SimulationData.h"

#include <math.h>
#include <random>
#include <time.h>

SimulationData::SimulationData( int gridLength, int gridWidth, int gridHeight )
	: x(gridLength), y(gridWidth), z(gridHeight) { 

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