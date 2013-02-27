#include "stdafx.h"

#include "RendererModule.h"

#include <SOIL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include "ShaderManager.h"

GLuint windowWidth = 800;
GLuint windowHeight = 600;
const char * windowTitle = "Real-timeish Cloud Renderer";

GLuint billboardShaderProgram;
GLuint vao;
GLuint circleTex;

RendererModule::RendererModule() {};

bool RendererModule::initialize( int gridX, int gridY, int gridZ ) {

	// Initialize GLFW and opens a window
	if( glfwInit() != GL_TRUE ) {
		return false;
	}

	glfwOpenWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
	glfwOpenWindowHint( GLFW_WINDOW_NO_RESIZE, GL_TRUE );
	glfwOpenWindowHint( GLFW_OPENGL_PROFILE, 0 );

	if (glfwOpenWindow( windowWidth, windowHeight, 0, 0, 0, 0, 24, 8, 
		GLFW_WINDOW ) != GL_TRUE) {
			glfwTerminate();
			return false;
	}
	glfwSetWindowTitle( windowTitle );

	// Initialize GLEW
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if(err != GLEW_OK) {
		std::cerr << "Failed to initialize GLEW: " << 
			glewGetErrorString(err) << "\n";
		exit( EXIT_FAILURE );
	}

	std::cout << "Running OpenGL version " << glGetString(GL_VERSION) << "\n";

	glGenVertexArrays( 1, &vao );
	glBindVertexArray( vao );

	// Load and compile shaders
	ShaderManager shaderManager;
	billboardShaderProgram = shaderManager.createFromFile( 
		"BillboardShader.vert", "BillboardShader.frag" );
	glUseProgram( billboardShaderProgram );
	
	// Generate the textures
	glGenTextures( 1, &circleTex );
	glBindTexture( GL_TEXTURE_2D, circleTex );

	int iWidth, iHeight;
	unsigned char* image;

	// Load the textures
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

	// A single billboard data
	float vertexSize = 1.8f;
	float vertices[] = {
	//	Vertex position		      Texcoords
		-vertexSize,  vertexSize, 0.0f, 1.0f,  		// Vertex 1 (-X,  Y)
		-vertexSize, -vertexSize, 0.0f, 0.0f,		// Vertex 2 (-X, -Y)
		 vertexSize,  vertexSize, 1.0f, 1.0f,		// Vertex 3 ( X,  Y)
		 vertexSize,  vertexSize, 1.0f, 1.0f,		// Vertex 3 ( X,  Y)
		-vertexSize, -vertexSize, 0.0f, 0.0f,		// Vertex 2 (-X, -Y)
		 vertexSize, -vertexSize, 1.0f, 0.0f		// Vertex 4 ( X, -Y)
	};
	GLuint vbo;
	glGenBuffers( 1, &vbo ); // Generate 1 buffer
	glBindBuffer( GL_ARRAY_BUFFER, vbo );
	glBufferData( GL_ARRAY_BUFFER, sizeof( vertices ), vertices, 
		GL_STATIC_DRAW );

	// Define data layout
	GLint posAttrib = glGetAttribLocation( billboardShaderProgram, 
		"vertPos" );
	glEnableVertexAttribArray( posAttrib );
	glVertexAttribPointer( posAttrib, 2, GL_FLOAT, GL_FALSE, 
		4*sizeof(float), 0 );

	GLint texAttrib = glGetAttribLocation( billboardShaderProgram, 
		"texCoord" );
	glEnableVertexAttribArray( texAttrib );
	glVertexAttribPointer( texAttrib, 2, GL_FLOAT, GL_FALSE, 
		4*sizeof(float), (void*)( 2*sizeof(float) ) );

	// Initialize the camera and the projection matrices
	camera.initialize( gridX, gridY, gridZ );
	perspectiveProjection = glm::perspective( 85.0f, 
		(float)windowWidth / (float)windowHeight, 0.4f, 300.0f );
	orthographicProjection = glm::ortho( -(windowWidth/2.0f), windowWidth/2.0f, 
		-(windowHeight/2.0f), windowHeight/2.0f, 0.0f, 1000.0f ); 

	// Set the sun position and sun position matrix (looking at the center of 
	// the cloud)
	sunPosition = glm::vec3( 300, 250, 250 ); 
	glm::vec3 lookAtPoint = glm::vec3( gridX/2, gridY/2, -gridZ/2 );
	sunTransformation = glm::lookAt( sunPosition, 
		lookAtPoint, glm::vec3(10, 10, 0));

	return true;

}

void RendererModule::draw( SimulationData* data, GLFWmutex simMutex, double time ) {

	// Update the camera
	camera.updateCamera();
	
	// Place the camera in the sun position
	GLint uniView = glGetUniformLocation( billboardShaderProgram, "view" );
	glUniformMatrix4fv( uniView, 1, GL_FALSE, glm::value_ptr( sunTransformation ) );

	// Set the parallel projection
	GLint uniProj = glGetUniformLocation( billboardShaderProgram, "proj" );
	glUniformMatrix4fv( uniProj, 1, GL_FALSE, glm::value_ptr( orthographicProjection ) );

	// Clear the screen with white color
	glClearColor( 1.0f, 1.0f, 1.0f, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	
	// Lock mutex because we will use data, which is shared with simulation
	glfwLockMutex( simMutex );

	shadeClouds( data, time );

	// Place the camera at the viewpoint
	glm::mat4 view = camera.getLookAtMatrix();
	//glUniformMatrix4fv( uniView, 1, GL_FALSE, glm::value_ptr( view ) );
	
	// Set perspective projection
	//glUniformMatrix4fv( uniProj, 1, GL_FALSE, glm::value_ptr( perspectiveProjection ) );

	// Clear the screen with background (sky) color
	glClearColor( 155/256.0f, 225/256.0f, 251/256.0f, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	
	renderClouds( data, time );

	glfwUnlockMutex( simMutex );

	// Check for errors
	GLint glErr = glGetError();
	if ( glErr ) std::cout << "OpenGL error " << glErr << "!\n";

	// Swap the buffer
	glfwSwapBuffers();

}

void RendererModule::shadeClouds( SimulationData* data, double time ) { }

void RendererModule::renderClouds( SimulationData* data, double time ) { 
	
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable( GL_BLEND );

	GLint uniPosition = glGetUniformLocation( billboardShaderProgram, "position" );
	GLint uniAlpha = glGetUniformLocation( billboardShaderProgram, "alpha" );

	int x = data->getGridLength();
	int y = data->getGridWidth();
	int z = data->getGridHeight();
	
	double startTime = glfwGetTime();

	// Calculate relative difference for linear interpolation
	float relDiff = (time - data->nextTime)/(data->nextTime - data->prevTime);
	if( relDiff > 1.0f )relDiff = 1.0f;

	for( int i = 0; i < x; ++i ) 
		for( int j = 0; j < y; ++j ) 
			for( int k = 0; k < z; ++k ) {
				if( data->nextDen[i][j][k] > 0.0f ) {

					// Lineary interpolate the density
					float density = data->prevDen[i][j][k] + relDiff
						* (data->nextDen[i][j][k] - data->prevDen[i][j][k] );

					if( density > 0.0f) {
						// Build a translation (model) matrix in the shader, 
						// because it's a lot faster than creating the matrix 
						//// here. 
						glUniform3f( uniPosition, i, j, -k );
						glUniform1f( uniAlpha, density );

						glDrawArrays( GL_TRIANGLES, 0, 6 );
					}
				}
			}
}

void RendererModule::terminate() {

	glDeleteProgram( billboardShaderProgram );
	// Todo : delete shaders
	glDeleteTextures( 1, &circleTex );
	glDeleteVertexArrays( 1, &vao );

	// Terminate GLFW
	glfwTerminate();

}