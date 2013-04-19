// Module responsible for drawing stuff

#ifndef RENDERERMODULE_H
#define RENDERERMODULE_H

#include "SimulationData.h"
#include "Camera.h"

class RendererModule {
public:
	RendererModule();
	bool initialize( const int gridX, const int gridY, const int gridZ );
	void terminate();
	void draw( const SimulationData* data, GLFWmutex dataMutex, 
		       const double frameTime );
	inline void changeShowSplat() { showSplat = !showSplat; };
	inline void changeShowVRC() { showVRC = !showVRC; };
private:
	void interpolateCloudData( const SimulationData* data, 
		const double frameTime );
	void renderSplattingClouds( const SimulationData* data, 
		const double frameTime ); //TODO: delete frametime
	void renderRayCastingClouds( const SimulationData* data, 
		const double frameTime );
	void defineBillboardLayout( const GLuint shaderProgram ); 
	void defineRaycasterLayout( const GLuint shaderProgram );

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