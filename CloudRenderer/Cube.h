#ifndef CUBE_H
#define CUBE_H

// Stores vertex and element data of a cube

void getCubeVertices( const float startX, const float stopX, 
					  const float startY, const float stopY, 
					  const float startZ, const float stopZ, 
					  float vertices [] );
void getCubeElements( int elements [] );

#endif CUBE_H