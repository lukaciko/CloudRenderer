#include "stdafx.h"

#include "RenderUtility.h"

#include <glm/gtc/type_ptr.hpp>

#include "SOIL.h"
#include "Globals.h"

GLuint VBOs[5];
GLuint numVBOs;
GLuint EBOs[5];
GLuint numEBOs;

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

void initializeTextures( GLuint volumeTexture, GLuint * planarTextures ) {
	// We need one 3D texture and several 2D textures
	
	// Generate the 2D textures using pngs
	glGenTextures( 3, planarTextures );

	int width, height;
	unsigned char* image;

	const char* files[] = { "SliderHandle.png", "SliderIndicator.png", "SliderNormal.png" };
	for( int i = 0; i < 2; ++i ) {
		image = SOIL_load_image( files[i], &width, &height, 0, SOIL_LOAD_RGBA );
		glBindTexture( GL_TEXTURE_2D, planarTextures[i] );
		glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, 
			GL_UNSIGNED_BYTE, image );
		SOIL_free_image_data( image );

		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	}

	// Generate the 3D textures
	glGenTextures( 1, &volumeTexture );
	glBindTexture( GL_TEXTURE_3D, volumeTexture );
	
	glTexParameteri( GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER  );
	glTexParameteri( GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER  );
	glTexParameteri( GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER  );
	glTexParameteri( GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glGenerateMipmap( GL_TEXTURE_3D );

}

void deleteTextures( GLuint volumeTexture, GLuint* planarTextures ) {
	glDeleteTextures( 1, &volumeTexture );
	glDeleteTextures( 2, planarTextures );
}

void setUniform( const std::string name, const float value ) {

	GLuint program;
    glGetIntegerv( GL_CURRENT_PROGRAM, (GLint*) &program );
	GLint location = glGetUniformLocation( program, name.c_str() );
	glUniform1f( location, value );

}

void setUniform( const std::string name, const glm::vec2 vector ) {

	GLuint program;
    glGetIntegerv( GL_CURRENT_PROGRAM, (GLint*) &program );
	GLint location = glGetUniformLocation( program, name.c_str() );
	glUniform2f( location, vector.x, vector.y );

}

void setUniform( const std::string name, const glm::vec3 vector ) {

	GLuint program;
    glGetIntegerv( GL_CURRENT_PROGRAM, (GLint*) &program );
	GLint location = glGetUniformLocation( program, name.c_str() );
	glUniform3f( location, vector.x, vector.y, vector.z );

}

void setUniform( const std::string name, const glm::mat4 matrix ) {

	GLuint program;
    glGetIntegerv( GL_CURRENT_PROGRAM, (GLint*) &program );
	GLint location = glGetUniformLocation( program, name.c_str() );
	glUniformMatrix4fv( location, 1, GL_FALSE, glm::value_ptr( matrix ) );
}

float convertXToRelative( const int x ) {
	return ( static_cast<float>( x ) / global_consts::windowWidth ) * 2.0f - 1.0f;
}

float convertYToRelative( const int y ) {
	return -(( static_cast<float>( y ) / global_consts::windowHeight ) * 2.0f - 1.0f);
}