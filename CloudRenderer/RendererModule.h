// Module responsible for drawing stuff

#ifndef RENDERERMODULE_H
#define RENDERERMODULE_H

#include "SimulationData.h"
#include "Camera.h"

class RendererModule {
public:
	RendererModule();
	bool initialize( int gridX, int gridY, int gridZ );
	void terminate();
	void draw( SimulationData* data, GLFWmutex dataMutex, double frameTime );
	inline void changeShowSplat() { showSplat = !showSplat; };
	inline void changeShowVRC() { showVRC = !showVRC; };
private:
	void interpolateCloudData( SimulationData* data, double frameTime );
	void renderSplattingClouds( SimulationData* data, double frameTime ); //TODO: delete frametime
	void renderRayCastingClouds( SimulationData* data, double frameTime );
	void defineBillboardLayout( GLuint shaderProgram ); 
	void defineRaycasterLayout( GLuint shaderProgram );

	bool showSplat;
	bool showVRC;

	float*** interpolatedData;

	Camera camera;
	glm::mat4 perspectiveProjection;
	glm::mat4 orthographicProjection;
	glm::vec3 sunPosition;
	glm::mat4 sunTransformation;
};

#endif