// A cellular automata approach to simulating clouds, such as described by 
// Dobashi et al., which extends Nagel's method

#ifndef SIMULATORMODULE_H
#define SIMULATORMODULE_H

#include "SimulationData.h"

class SimulatorModule {
public:
	SimulatorModule();
	void initialize();
	void terminate();
	// Part of the simulation step that can be done asynchronously
	void stepAsych( SimulationData* data );
	// Part of the simulation step that updates shared data and is protected with
	// a mutex
	void stepMutex( SimulationData* data, double startTime );
private:
	// Vapor extinction probability
	float pHumExt;
	// Phase transition extinction probability
	float pActExt;
	// Cloud extinction probability
	float pCldExt;
	int randomResolution;

	// Simulate a cellular automata step
	void simulateCellular( int x, int y, int z, bool *** hum, bool
		*** act, bool *** cld, bool *** fAc );
	// Calculate continous cloud density distribution for entire grid
	void calculateDensity( int x, int y, int z, 
		bool *** cld, float *** den );
	// Calculate continous cloud density distribution for one cell
	float singleDensity( int x, int y, int z, int i, int j, int k,
		bool *** cld, int S );
	// Calculate field function
	float fieldFunction( float a );
	// Override one array with another
	void copyGrid( float*** copyTo, float*** copyFrom, int x, int y, int z );

};

#endif