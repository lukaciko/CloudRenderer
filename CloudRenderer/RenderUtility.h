// Abstract some OpenGL calls, provides utility functions

#ifndef RENDERUTILITY_H
#define RENDERUTILITY_H

#include <glm\glm.hpp>
#include <string>

GLuint createVBO( float vertices [], int size );
GLuint createEBO( int elements [], int size );
void deleteVBOs();
void deleteEBOs();
void initializeTextures( GLuint volumeTexture, GLuint* planarTextures );
void deleteTextures( GLuint volumeTexture, GLuint* planarTextures );
void setUniform( const std::string name, const float value );
void setUniform( const std::string name, const glm::vec2 vector );
void setUniform( const std::string name, const glm::vec3 vector );
void setUniform( const std::string name, const glm::mat4 matrix );
float convertXToRelative( const int x );
float convertYToRelative( const int y );

#endif