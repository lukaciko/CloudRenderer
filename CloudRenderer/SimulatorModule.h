// A cellular automata approach to simulating clouds, such as described by 
// Dobashi et al., which extends Nagel's method

#ifndef SIMULATORMODULE_H
#define SIMULATORMODULE_H

#include "SimulationData.h"
#include "Cloud.h"

#include <random>
#include <deque>

class SimulatorModule {
public:
	SimulatorModule( const int x, const int y, const int z );
	// Part of the simulation step that can be done asynchronously
	void stepAsych( SimulationData* data );
	// Part of the simulation step that updates shared data and is protected with
	// a mutex
	void stepMutex( SimulationData* data, const double startTime );
private:
	typedef std::deque<Cloud> CV;

	const int x;
	const int y;
	const int z;

	// Smaller base gives bigger clouds that more closely resemble elepsoids
	const int exponentialBase;
	const int minCloudSize;
	const int maxCloudSize;
	
	// List of clouds
	CV clouds;
	// Vapor extinction probability
	float pHumExt;
	// Phase transition extinction probability
	float pActExt;
	// Cloud extinction probability
	float pCldExt;

	std::mt19937 gen;
	int randomResolution;

	// Simulate a cellular automata step
	void simulateCellular( bool *** hum, bool
		*** act, bool *** cld, bool *** fAc, float *** distSize );
	// Creates a cloud at random position with random size
	void createRandomCloud();
	// Recalculates dist/size, should be called when clouds are created
	void calculateDistSite( float *** distSize );
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