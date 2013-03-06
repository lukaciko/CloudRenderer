#include "stdafx.h"

#include "Cube.h"

// Float array must be of size 3x8 floats
void getCubeVertices( int startX, int stopX, int startY, int stopY, 
					 int startZ, int stopZ, float vertices [] ) {

	float tempVertices [] = {
		startX, startY, -startZ,
		stopX,  startY, -startZ,
		startX, stopY,  -startZ,
		stopX,  stopY,  -startZ, 
		startX, startY, -stopZ, 
		stopX,  startY, -stopZ, 
		startX, stopY,  -stopZ, 
		stopX,  stopY,  -stopZ 
	};
	for( int i = 0; i < 24; ++i ) {
		vertices[i] = tempVertices[i];
	}

}

// Elements must have size 36 (6 faces, 2 triangles per face with 3 points)
void getCubeElements( int elements [] ) {

	int tempElements [] = {
		0, 1, 2,
		2, 1, 3,
		4, 6, 5,
		6, 7, 5,
		1, 5, 3,
		3, 5, 7,
		0, 2, 4,
	    2, 6, 4,
		6, 2, 3,
		6, 3, 7,
		1, 0, 4,
		5, 1, 4
	};
	for( int i = 0; i < 36; ++i ) {
		elements[i] = tempElements[i];
	}
}