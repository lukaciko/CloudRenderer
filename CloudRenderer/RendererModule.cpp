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

// Initializes the RendererModule
// Returns false if it fails to initialize
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

	// data for shader
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

	// Initialize the camera and the projection matrix
	camera.initialize( gridX, gridY, gridZ );
	glm::mat4 proj = glm::perspective( 60.0f, 
		(float)windowWidth / (float)windowHeight, 0.4f, 300.0f );
	GLint uniProj = glGetUniformLocation( billboardShaderProgram, "proj" );
	glUniformMatrix4fv( uniProj, 1, GL_FALSE, glm::value_ptr( proj ) );

	return true;

}

void RendererModule::draw( SimulationData* data, GLFWmutex simMutex, double time ) {

	// Clear the screen with sky color
	glClearColor( 155/256.0f, 225/256.0f, 251/256.0f, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable( GL_BLEND );

	// Update the camera
	camera.updateCamera();

	glm::mat4 view = camera.getLookAtMatrix();
	GLint uniView = glGetUniformLocation( billboardShaderProgram, "view" );
	glUniformMatrix4fv( uniView, 1, GL_FALSE, glm::value_ptr( view ) );

	GLint uniPosition = glGetUniformLocation( billboardShaderProgram, "position" );
	GLint uniAlpha = glGetUniformLocation( billboardShaderProgram, "alpha" );
	
	// Calculate relative difference for linear interpolation
	float relDiff = (time - data->nextTime)/(data->nextTime - data->prevTime);
	if( relDiff > 1.0f )relDiff = 1.0f;

	int x = data->getGridLength();
	int y = data->getGridWidth();
	int z = data->getGridHeight();

	// Lock mutex and draw stuff
	glfwLockMutex( simMutex );
	double startTime = glfwGetTime();
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
						//float posX = ( i - x / 2.0f );
						//float posY = ( j - y / 2.0f );
						//float posZ = ( k - z / 2.0f );
						
						glUniform3f( uniPosition, i, j, -k );
						glUniform1f( uniAlpha, density );
						
						glDrawArrays( GL_TRIANGLES, 0, 6 );
					}
				}
			}

	glfwUnlockMutex( simMutex );

	GLint glErr = glGetError();
	if ( glErr ) std::cout << "OpenGL error " << glErr << "!\n";

	// Swap the buffer
	glfwSwapBuffers();

}

void RendererModule::terminate() {

	glDeleteProgram( billboardShaderProgram );
	// Todo : delete shaders
	glDeleteTextures( 1, &circleTex );
	glDeleteVertexArrays( 1, &vao );

	// Terminate GLFW
	glfwTerminate();

}