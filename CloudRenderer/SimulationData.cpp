#include "stdafx.h"

#include "SimulationData.h"

#include <math.h>
#include <time.h>

SimulationData::SimulationData( const int gridLength, const int gridWidth, 
							   const int gridHeight )
	: x(gridLength), y(gridWidth), z(gridHeight) { 

	// Allocation of grids
	hum = new bool ** [gridLength];
	cld = new bool ** [gridLength];
	act = new bool ** [gridLength];
	fAc = new bool ** [gridLength];
	distSize = new float ** [gridLength];
	workDen = new float ** [gridLength];
	prevDen = new float ** [gridLength];
	nextDen = new float ** [gridLength];

	make3Darray( hum );
	make3Darray( cld );
	make3Darray( act );
	make3Darray( fAc );
	make3Darray( distSize );
	make3Darray( workDen );
	make3Darray( prevDen );
	make3Darray( nextDen );

	prevTime = 0; // TODO: set to glfw time
	nextTime = 0;

}

SimulationData::~SimulationData() {

	// Deallocate the array memory and delete the pointers
	delete3Darray( hum );
	delete3Darray( cld );
	delete3Darray( act );
	delete3Darray( fAc );
	delete3Darray( distSize );
	delete3Darray( workDen );
	delete3Darray( prevDen );
	delete3Darray( nextDen );
	delete hum;
	delete cld;
	delete act;
	delete fAc;
	delete distSize;
	delete workDen;
	delete prevDen;
	delete nextDen;

}


template <typename T>
void SimulationData::make3Darray( T *** arr ) {

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
void SimulationData::delete3Darray( T *** arr ) {

	// Deallocation of a 3d array
	for( int i = 0; i != x; ++i ) {
		for (int j = 0; j != y; ++j ) 
			delete [] arr[i][j];
		delete [] arr[i];
	}

}