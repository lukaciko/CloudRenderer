// Module responsible for drawing stuff

#ifndef RENDERERMODULE_H
#define RENDERERMODULE_H

#include "SimulationData.h"

#define GLFW_DLL

#include <GL/glew.h>
#include <GL/glfw.h>

#include "Camera.h"

class RendererModule {
public:
	RendererModule();
	bool initialize( int, int, int );
	void terminate();
	void draw( SimulationData*, GLFWmutex, double );
private:
	Camera camera;
};

#endif