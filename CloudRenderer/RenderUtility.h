// Abstract some OpenGL calls, provides utility functions

#ifndef RENDERUTILITY_H
#define RENDERUTILITY_H

#include <glm\glm.hpp>

GLuint createVBO( float [], int );
GLuint createEBO( int [], int );
void deleteVBOs();
void deleteEBOs();
void initializeTextures();
void deleteTextures();
void setUniform( const char*, glm::mat4 );

#endif