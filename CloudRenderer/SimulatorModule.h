// A cellular automata approach to simulating clouds, such as described by 
// Dobashi et al., which extends Nagel's method

#ifndef SIMULATORMODULE_H
#define SIMULATORMODULE_H

#include "SimulationData.h"
#include "Cloud.h"

#include <vector>

class SimulatorModule {
public:
	SimulatorModule( const int x, const int y, const int z );
	// Part of the simulation step that can be done asynchronously
	void stepAsych( SimulationData* data );
	// Part of the simulation step that updates shared data and is protected with
	// a mutex
	void stepMutex( SimulationData* data, const double startTime );
private:
	typedef std::vector<Cloud> CV;

	const int x;
	const int y;
	const int z;

	// List of clouds
	CV clouds;
	// Vapor extinction probability
	float pHumExt;
	// Phase transition extinction probability
	float pActExt;
	// Cloud extinction probability
	float pCldExt;
	int randomResolution;

	// Simulate a cellular automata step
	void simulateCellular( bool *** hum, bool
		*** act, bool *** cld, bool *** fAc, float *** distSize );
	// Creates a cloud at random position with random size, recalculates 
	// dist/size ratios
	void createRandomCloud( float *** distSize );
	// Calculate continous cloud density distribution for entire grid
	void calculateDensity( bool *** cld, float *** den );
	// Calculate continous cloud density distribution for one cell
	float singleDensity( int i, int j, int k,
		bool *** cld, int S );
	// Calculate field function
	float fieldFunction( float a );
	// Override one array with another
	void copyGrid( float*** copyTo, float*** copyFrom );

};

#endif