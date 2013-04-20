// Stores the simulation data, in particular a grid representing various cloud
// data

#ifndef SIMULATIONDATA_H
#define SIMULATIONDATA_H

#include <vector>

class SimulationData {
	friend class SimulatorModule;
public:
	SimulationData( const int gridLength, const int gridWidth, 
		const int gridHeight ); 
	~SimulationData();

	float *** nextDen; // Continous density distribution (protected with mutex)
	GLfloat *** prevDen; // Previous continous density distribution

	double nextTime; // Time when nextDen was computed
	double prevTime; // Time when prevDen was computed

	inline int getGridLength() const { return x; }
	inline int getGridWidth() const { return y; }
	inline int getGridHeight() const { return z; }
private:

	const int x; // The length of the grid
	const int y; // The width of the grid
	const int z; // The height of the grid
	
	bool *** hum; // Humidity
	bool *** cld; // Clouds
	bool *** act; // Phase transition/activation
	bool *** fAc;

	float *** distSize; // Private size/distance to nearest cloud ratio
	float *** workDen; // Private continous density distribution

	// Creates a dynamic 3d boolean array of size x*y*z
	template <typename T>
	void make3Darray( T *** type );
	
	// Frees the 3d boolean array memory
	template <typename T>
	void delete3Darray( T *** type );
};

#endif