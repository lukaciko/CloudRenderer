#include "stdafx.h"

#include "RenderUtility.h"

#include <glm/gtc/type_ptr.hpp>

void setUniform( const char* name, glm::mat4 matrix ) {

	GLuint program;
    glGetIntegerv( GL_CURRENT_PROGRAM, (GLint*) &program );
	GLint location = glGetUniformLocation( program, name );
	glUniformMatrix4fv( location, 1, GL_FALSE, glm::value_ptr( matrix ) );

}