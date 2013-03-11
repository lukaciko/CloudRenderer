#include "stdafx.h"

#include "mainLoop.h"

#include <iostream>

#include "SimulationData.h"
#include "RendererModule.h"
#include "SimulatorModule.h"

namespace loop {

	// The size of the grid (must be powers of 2)
	int gridX = 128;
	int gridY = 128;
	int gridZ = 128;

	// The radius of metaballs
	float R = 1;

	int frameCap = 60;
	double simulationCap = 2;
	
	SimulationData* simulationData;
	SimulatorModule* simulatorModule;
	RendererModule* rendererModule;

	// Simulation thread
	GLFWthread simThread;
	// Final simulation data mutex
	GLFWmutex simMutex;
	
	// True when program begins exit
	bool exit = false;

	bool simPaused = false;

	// Function that asynchronously runs simulation 
	void GLFWCALL Simulate( void *arg) {
		
		while(!exit) {
			if( !simPaused ) {
				
				double startTime = glfwGetTime();

				// Perform the part of the simulation that can be done 
				// asynchronously
				simulatorModule->stepAsych( simulationData );

				// Lock mutex and do the rest of the simulation
				glfwLockMutex( simMutex );
				simulatorModule->stepMutex( simulationData, glfwGetTime() );
				glfwUnlockMutex( simMutex );

				glfwSleep( 1.0/simulationCap - glfwGetTime() + startTime );

			}
		}
	}

	void GLFWCALL keyCallback( int key, int state ) {
		if( key=='P' && state == GLFW_PRESS )
			simPaused = !simPaused;
	}

	void run() {

		// Allocate simulation data on the heap
		simulationData = new SimulationData( gridX, gridY, gridZ, R );

		// Initialize cloud renderer module
		rendererModule = new RendererModule();
		if( !rendererModule->initialize( gridX, gridY, gridZ ) )
			return;

		// Initialize cloud simulation module
		simulatorModule = new SimulatorModule( R );
		simulatorModule->initialize();
		
		void* arguments [] = { &simulationData, &simulatorModule };

		// Create the simulation thread and data mutex
		simThread = glfwCreateThread( Simulate, arguments );
		simMutex = glfwCreateMutex();

		glfwSetKeyCallback( keyCallback );

		// The main loop
		while( glfwGetWindowParam( GLFW_OPENED ) ) {

			double startTime = glfwGetTime();

			rendererModule->draw( simulationData, simMutex, startTime );
		    
			glfwSleep( 1.0/frameCap - glfwGetTime() + startTime );
		
		}

		exit = true;

		// Wait for the other thread to finish
		glfwWaitThread( simThread, GLFW_WAIT );

		// Terminate everything in opposite order of initialization

		simulatorModule->terminate();
		delete simulatorModule;

		rendererModule->terminate();
		delete rendererModule;

		delete simulationData;

	}
}