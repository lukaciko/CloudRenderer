#include "stdafx.h"

#include "RenderUtility.h"

#include <SOIL.h>
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
	// We can get away with such initialization because we only need one 3D and 
	// one 2D texture
	
	// Generate the textures
	glGenTextures( 1, &circleTexture );
	glBindTexture( GL_TEXTURE_2D, circleTexture );
	glGenTextures( 1, &volumeTexture );
	glBindTexture( GL_TEXTURE_3D, volumeTexture );

	int iWidth, iHeight;
	unsigned char* image;

	// Load the particle texture
	const char* path = "particle_texture.jpg";
	image = SOIL_load_image( path, &iWidth, &iHeight, 0, SOIL_LOAD_RGBA );
	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, iWidth, iHeight, 0, GL_RGBA,
		GL_UNSIGNED_BYTE, image );
	SOIL_free_image_data( image );
	
	// Select OpenGL texture parameters
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glGenerateMipmap( GL_TEXTURE_2D );

	glTexParameteri( GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE  );
	glTexParameteri( GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE  );
	glTexParameteri( GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE  );
	glTexParameteri( GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glGenerateMipmap( GL_TEXTURE_3D );

}

void deleteTextures() {
	glDeleteTextures( 1, &circleTexture );
	glDeleteTextures( 1, &volumeTexture );
}


void setUniform( const char* name, glm::vec3 vector ) {

	GLuint program;
    glGetIntegerv( GL_CURRENT_PROGRAM, (GLint*) &program );
	GLint location = glGetUniformLocation( program, name );
	glUniform3f( location, vector.x, vector.y, vector.z );

}

void setUniform( const char* name, glm::mat4 matrix ) {

	GLuint program;
    glGetIntegerv( GL_CURRENT_PROGRAM, (GLint*) &program );
	GLint location = glGetUniformLocation( program, name );
	glUniformMatrix4fv( location, 1, GL_FALSE, glm::value_ptr( matrix ) );

}