// Module responsible for drawing stuff

#ifndef RENDERERMODULE_H
#define RENDERERMODULE_H

#include "SimulationData.h"
#include "Camera.h"
#include "GUIControls.h"

class RendererModule {
public:
	RendererModule();
	bool initialize( const int gridX, const int gridY, const int gridZ );
	void terminate();
	void draw( const SimulationData& data, GLFWmutex dataMutex, 
		       const double frameTime );
	inline void changeShowVRC() { showVRC = !showVRC; };
private:
	void interpolateCloudData( const SimulationData& data, 
		const double frameTime );
	void renderRayCastingClouds( const SimulationData& data, 
		const double frameTime );
	void defineRaycasterLayout( const GLuint shaderProgram );
	void defineGUILayout( const GLuint shaderProgram );

	bool showVRC;

	float*** interpolatedData;

	Camera camera;
	GUIControls controls;
	glm::mat4 perspectiveProjection;
	glm::vec3 sunPosition;
	glm::mat4 sunTransformation;
};

#endif