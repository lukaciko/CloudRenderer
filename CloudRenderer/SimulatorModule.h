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
	void stepMutex( SimulationData* data, double );
private:
	// Vapor extinction probability
	float pHumExt;
	// Phase transition extinction probability
	float pActExt;
	// Cloud extinction probability
	float pCldExt;
	int randomResolution;

	// Simulate a cellular automata step
	void simulateCellular( int, int, int, bool ***, 
		bool ***, bool ***, bool *** );
	// Calculate continous cloud density distribution for entire grid
	void calculateDensity( int, int, int, bool ***, float *** );
	// Calculate continous cloud density distribution for one cell
	float singleDensity( int, int, int, int, int, int, bool ***, int);
	// Calculate field function
	float fieldFunction( float );
	// Override one array with another
	void copyGrid( float***, float*** , int, int, int );

};

#endif