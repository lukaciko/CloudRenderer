#include "stdafx.h"

#include "RendererModule.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include "ShaderManager.h"
#include "RenderUtility.h"
#include "Cube.h"

GLuint windowWidth = 1200;
GLuint windowHeight = 900;
const char * windowTitle = "Real-timeish Cloud Renderer";

ShaderManager shaderManager;
GLuint raycasterShaderProgram;
GLuint VAOs [1];
GLuint cubeVBO;

float nearPlane = 0.1f;
float farPlane = 25.0f;

float fieldOfView = 85.0f;
float tanFOV = tan( fieldOfView / 2.0f / 360 * 2 * 3.14f );

RendererModule::RendererModule() {
	showVRC = true;
};

bool RendererModule::initialize( const int gridX, const int gridY, 
								 const int gridZ ) {

	// Initialize GLFW and opens a window
	if( glfwInit() != GL_TRUE ) {
		return false;
	}

	/*glfwOpenWindowHint( GLFW_OPENGL_VERSION_MAJOR, 3 );
	glfwOpenWindowHint( GLFW_OPENGL_VERSION_MINOR, 2 );
	glfwOpenWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );*/
	glfwOpenWindowHint( GLFW_WINDOW_NO_RESIZE, GL_TRUE );

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
		return false;
	}
	// Ignore the OpenGL error (1280) that glewInit() causes
	glGetError();

	std::cout << "Running OpenGL version " << glGetString(GL_VERSION) << "\n";

	glGenVertexArrays( 1, VAOs );
	glBindVertexArray( VAOs[0] );

	// Load and compile shaders
	raycasterShaderProgram = shaderManager.createFromFile( 
		"RaycasterShader.vert", "RaycasterShader.frag" );

	initializeTextures();
	
	// Create cube that encapsulates the grid for ray casting
	float cubeVertices[24];
	getCubeVertices( 0, 1, 0, 1, 0, 1, cubeVertices );

	cubeVBO = createVBO( cubeVertices, sizeof( cubeVertices )) ;
	glBindVertexArray( VAOs[0] );
	defineRaycasterLayout( raycasterShaderProgram );

	int cubeElements[36];
	getCubeElements( cubeElements );
	createEBO( cubeElements, sizeof( cubeElements ));

	// Initialize the camera and the projetion matrices
	camera.initialize( gridX, gridY, gridZ );
	perspectiveProjection = glm::perspective( 85.0f, 
		(float)windowWidth / (float)windowHeight, nearPlane, farPlane );
		
	interpolatedData = new float ** [gridX];
	for( int i = 0; i != gridX; ++i ) {
		interpolatedData[i] = new float*[gridY];
		for (int j = 0; j != gridY; ++j ) 
			interpolatedData[i][j] = new float[gridZ];
	}

	// Initialize the sliders
	controls.addSlider( "Slider1", "densityCutoff", 0.0f, 1.0f );

	return true;

}

void RendererModule::defineRaycasterLayout( const GLuint raycasterShaderProgram ) {

	GLint posAttrib = glGetAttribLocation( raycasterShaderProgram, 
		"cubeVert" );
	glEnableVertexAttribArray( posAttrib );
	glVertexAttribPointer( posAttrib, 3, GL_FLOAT, GL_FALSE, 
		3*sizeof(float), 0 );

}

void RendererModule::draw( const SimulationData& data, GLFWmutex simMutex, 
						   const double time ) {
	
	// Update the camera
	camera.updateCamera();
	controls.update();

	// Clear the screen with background (sky) color
	glClearColor( 155/256.0f, 225/256.0f, 251/256.0f, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	
	// Lock mutex because we will use data, which is shared with simulation
	glfwLockMutex( simMutex );

	interpolateCloudData( data, time );

	if( showVRC )
		renderRayCastingClouds( data, time );
	
	glfwUnlockMutex( simMutex );

	// Render the controls in orthographic mode
	controls.render();

	// Check for errors
	GLint glErr = glGetError();
	if ( glErr ) std::cout << "OpenGL error " << glErr << "!\n";

	// Swap the buffer
	glfwSwapBuffers();

}

void RendererModule::interpolateCloudData( const SimulationData & data,  
										  const double time ) {

	int x = data.getGridLength();
	int y = data.getGridWidth();
	int z = data.getGridHeight();

	// Calculate relative difference for linear interpolation
	float relDiff = (time - data.nextTime)/(data.nextTime - data.prevTime);
	if( relDiff > 1.0f )relDiff = 1.0f;

	for( int i = 0; i < x; ++i ) 
		for( int j = 0; j < y; ++j ) 
			for( int k = 0; k < z; ++k )
				if( data.nextDen[i][j][k] > 0.0f ) {

					// Lineary interpolate the density
					interpolatedData[i][j][k] = data.prevDen[i][j][k] + relDiff
						* (data.nextDen[i][j][k] - data.prevDen[i][j][k] );

				}
				else
					interpolatedData[i][j][k] = 0.0f;
}

// Shade clouds by performing volume ray casting
void RendererModule::renderRayCastingClouds( const SimulationData & data, 
											const double time ) {

	glBindVertexArray( VAOs[0] );
	glUseProgram( raycasterShaderProgram );
	setUniform( "view", camera.getLookAtMatrix() );
	setUniform( "viewInverse", glm::inverse(camera.getLookAtMatrix()) );
	setUniform( "proj", perspectiveProjection );
	setUniform( "tanFOV", tanFOV );
	setUniform( "screenSize", glm::vec2( windowWidth, windowHeight ) );
	setUniform( "eyePosition", camera.getEyeLocation() );
	setUniform( "near", nearPlane );
	setUniform( "far", farPlane );

	glDisable( GL_CULL_FACE );
	glEnable( GL_DEPTH_TEST );

	int x = data.getGridLength();
	int y = data.getGridWidth();
	int z = data.getGridHeight();

	// Convert float*** to float* (stream of data)
	float* texData = new float[x*y*z];
	int pos = 0;
	for( int i = 0; i != x; ++i )
		for( int j = 0; j != y; ++j ) 
			for( int k = 0; k != z; ++k ) {
				texData[pos] = interpolatedData[i][j][k];
				++pos;
			}

	// Fill the data into 3D texture. A texture cell includes only one
	// component (GL_RED = density, float). 
	glTexImage3D( GL_TEXTURE_3D, 0, GL_R32F, x, y, z, 0, GL_RED, 
		GL_FLOAT, texData );

	delete[] texData;

	glDrawElements( GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0 );

} 

void RendererModule::terminate() {

	shaderManager.terminate();
	glDeleteVertexArrays( 2, VAOs );
	deleteTextures();

	// Terminate GLFW
	glfwTerminate();

}