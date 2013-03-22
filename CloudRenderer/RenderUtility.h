// Abstract some OpenGL calls, provides utility functions

#ifndef RENDERUTILITY_H
#define RENDERUTILITY_H

#include <glm\glm.hpp>

GLuint createVBO( float vertices [], int size );
GLuint createEBO( int elements [], int size );
void deleteVBOs();
void deleteEBOs();
void initializeTextures();
void deleteTextures();
void setUniform( const char* name, glm::vec3 vector );
void setUniform( const char* name, glm::mat4 matrix );

#endif