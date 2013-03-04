#include "stdafx.h"

#include "Cube.h"

// Float array must be of size 3x8 floats
void getCubeVertices( int startX, int stopX, int startY, int stopY, 
					 int startZ, int stopZ, float vertices [] ) {
	
	vertices[0] = startX;
	vertices[1] = startY;
	vertices[2] = startZ;
	
	vertices[3] = stopX;
	vertices[4] = startY;
	vertices[5] = startZ;

	vertices[6] = startX;
	vertices[7] = stopY;
	vertices[8] = startZ;

	vertices[9] = stopX;
	vertices[10] = stopY;
	vertices[11] = startZ;

	vertices[12] = startX;
	vertices[13] = startY;
	vertices[14] = stopZ;
	
	vertices[15] = stopX;
	vertices[16] = startY;
	vertices[17] = stopZ;

	vertices[18] = startX;
	vertices[19] = stopY;
	vertices[20] = stopZ;

	vertices[21] = stopX;
	vertices[22] = stopY;
	vertices[23] = stopZ;

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
	    2, 4, 6,
		6, 2, 3,
		6, 3, 7,
		1, 0, 4,
		5, 1, 6
	};
	for( int i = 0; i < 36; ++i ) {
		elements[i] = tempElements[i];
	}
}