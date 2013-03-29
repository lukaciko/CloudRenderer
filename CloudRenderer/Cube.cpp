#include "stdafx.h"

#include "Cube.h"

// Float array must be of size 2x3x8 floats
void getCubeVertices( float startX, float stopX, float startY, float stopY, 
					 float startZ, float stopZ, float vertices [] ) {

	float tempVertices [] = {
		startX, startY, startZ, 0.0f, 0.0f, 0.0f,
		stopX,  startY, startZ, 1.0f, 0.0f, 0.0f,
		startX, stopY,  startZ, 0.0f, 1.0f, 0.0f,
		stopX,  stopY,  startZ, 1.0f, 1.0f, 0.0f,
		startX, startY, stopZ,  0.0f, 0.0f, 1.0f,
		stopX,  startY, stopZ,  1.0f, 0.0f, 1.0f,
		startX, stopY,  stopZ,  0.0f, 1.0f, 1.0f,
		stopX,  stopY,  stopZ,  1.0f, 1.0f, 1.0f
	};
	for( int i = 0; i < 48; ++i ) {
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