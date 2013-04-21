#include "stdafx.h"

#include "RenderUtility.h"

#include <glm/gtc/type_ptr.hpp>

GLuint VBOs[5];
GLuint numVBOs;
GLuint EBOs[5];
GLuint numEBOs;

GLuint circleTexture;
GLuint volumeTexture;

// Need to pass sizeof(vertices) as well because of dynamic arrays
GLuint createVBO( float vertices [], int size ) {

    glGenBuffers( 1, &VBOs[numVBOs] );
    glBindBuffer( GL_ARRAY_BUFFER, VBOs[numVBOs] );
    glBufferData( GL_ARRAY_BUFFER, size, vertices, 
		GL_STATIC_DRAW );
    return VBOs[numVBOs++];

}

GLuint createEBO( int elements [], int size ) {

    glGenBuffers( 1, &EBOs[numEBOs] );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, EBOs[numEBOs] );
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, size, elements, 
		GL_STATIC_DRAW );
    return EBOs[numEBOs++];

}

void deleteVBOs() {
	glDeleteBuffers( numVBOs, VBOs );
}

void deleteEBOs() {
	glDeleteBuffers( numEBOs, EBOs );
}

void initializeTextures() {
	// We can get away with such initialization because we only need one 3D 
	// texture
	
	// Generate the textures
	glGenTextures( 1, &volumeTexture );
	glBindTexture( GL_TEXTURE_3D, volumeTexture );
	
	glTexParameteri( GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER  );
	glTexParameteri( GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER  );
	glTexParameteri( GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER  );
	glTexParameteri( GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glGenerateMipmap( GL_TEXTURE_3D );

}

void deleteTextures() {
	glDeleteTextures( 1, &circleTexture );
	glDeleteTextures( 1, &volumeTexture );
}

void setUniform( const char* name, const float value ) {

	GLuint program;
    glGetIntegerv( GL_CURRENT_PROGRAM, (GLint*) &program );
	GLint location = glGetUniformLocation( program, name );
	glUniform1f( location, value );

}

void setUniform( const char* name, const glm::vec2 vector ) {

	GLuint program;
    glGetIntegerv( GL_CURRENT_PROGRAM, (GLint*) &program );
	GLint location = glGetUniformLocation( program, name );
	glUniform2f( location, vector.x, vector.y );

}

void setUniform( const char* name, const glm::vec3 vector ) {

	GLuint program;
    glGetIntegerv( GL_CURRENT_PROGRAM, (GLint*) &program );
	GLint location = glGetUniformLocation( program, name );
	glUniform3f( location, vector.x, vector.y, vector.z );

}

void setUniform( const char* name, const glm::mat4 matrix ) {

	GLuint program;
    glGetIntegerv( GL_CURRENT_PROGRAM, (GLint*) &program );
	GLint location = glGetUniformLocation( program, name );
	glUniformMatrix4fv( location, 1, GL_FALSE, glm::value_ptr( matrix ) );

}