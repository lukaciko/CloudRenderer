#include "stdafx.h"

#include "ShaderManager.h"

#include <GL\glew.h>

#include <string>
#include <iostream>
#include <fstream>

using namespace std;

std::string loadShader( char* filename ) {

	std::string line, text;
	std::ifstream in( filename );
	while( std::getline( in, line ) ) {
		text += line + "\n";
	}
	return text;

}

GLuint ShaderManager::createFromFile( char* vertexFile, char* fragmentFile ) {

	// Load shader source from file
	std::string vstring = loadShader( vertexFile );
	const char* vertexShaderSource = vstring.c_str();
	std::string fstring = loadShader( fragmentFile );
	const char* fragmentShaderSource = fstring.c_str();
	
	// Create and compile the vertex shader
	vertexShader = glCreateShader( GL_VERTEX_SHADER );
	glShaderSource( vertexShader, 1, &vertexShaderSource, NULL );
	glCompileShader( vertexShader );

	// Create and compile the fragment shader
	fragmentShader = glCreateShader( GL_FRAGMENT_SHADER );
	glShaderSource( fragmentShader, 1, &fragmentShaderSource, NULL );
	glCompileShader( fragmentShader );

	// Link the vertex and fragment shader into a shader program
	GLuint shaderProgram = glCreateProgram();
	glAttachShader( shaderProgram, vertexShader );
	glAttachShader( shaderProgram, fragmentShader );
	glBindFragDataLocation( shaderProgram, 0, "outColor" );
	glLinkProgram( shaderProgram );

	GLint abort = 0;
	GLint testVal;
	// Catch any errors
	glGetShaderiv( vertexShader, GL_COMPILE_STATUS, &testVal );
	if( testVal == GL_FALSE ) {
		abort = 1;
		char infolog[1024];
		glGetShaderInfoLog( vertexShader,1024,NULL,infolog );
		std::cerr << "The vertex shader failed to compile with the error:" << infolog;
	}

	glGetShaderiv( fragmentShader, GL_COMPILE_STATUS, &testVal );
	if( testVal == GL_FALSE ) {
		abort = 1;
		char infolog[1024];
		glGetShaderInfoLog( fragmentShader, 1024, NULL, infolog );
		std::cerr << "The fragment shader failed to compile with the error:" << infolog;
	}

	glGetProgramiv( shaderProgram, GL_LINK_STATUS, &testVal );
	if( testVal == GL_FALSE ) {
		abort = 1;
		char infolog[1024];
		glGetProgramInfoLog( shaderProgram, 1024, NULL, infolog );
		std::cerr << "The program failed to compile with the error:" << infolog;
	}
	if( abort ) {
		std::cerr << "errors occured, cannot continue, aborting.";
	}

	return shaderProgram;

}