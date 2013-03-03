// Module responsible for drawing stuff

#ifndef RENDERERMODULE_H
#define RENDERERMODULE_H

#include "SimulationData.h"
#include "Camera.h"

class RendererModule {
public:
	RendererModule();
	bool initialize( int, int, int );
	void initializeTextures();
	void terminate();
	void deleteTextures();
	void draw( SimulationData*, GLFWmutex, double );
	void shadeClouds( SimulationData*, double );
	void renderClouds( SimulationData*, double );
private:
	Camera camera;
	glm::mat4 perspectiveProjection;
	glm::mat4 orthographicProjection;
	glm::vec3 sunPosition;
	glm::mat4 sunTransformation;
};

#endif