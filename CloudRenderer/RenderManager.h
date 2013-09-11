// Module responsible for drawing stuff

#ifndef RENDERMANAGER_H
#define RENDERMANAGER_H

#include "SimulationData.h"
#include "Camera.h"
#include "GUIControls.h"
#include "ShaderManager.h"

class RenderManager {
public:
	RenderManager();
	bool initialize( const int gridX, const int gridY, const int gridZ );
	void terminate();
	void draw( const SimulationData& data, GLFWmutex dataMutex, 
		       const double frameTime );
	inline void changeShowVRC() { showVRC = !showVRC; };
private:
	const char * windowTitle;

	ShaderManager shaderManager;
	GLuint raycasterShaderProgram;
	GLuint guiShaderProgram;
	GLuint VAOs[2];
	GLuint cubeVBO;
	GLuint guiVBO;

	GLuint volumeTexture;
	GLuint planarTextures[3];

	const float nearPlane;
	const float farPlane;

	const float fieldOfView;
	const float tanFOV;

	void interpolateCloudData( const SimulationData& data, 
		const double frameTime );
	void renderRayCastingClouds( const SimulationData& data, 
		const double frameTime );
	void renderGUI();
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