// Takes care of camera manipulation

#ifndef CAMERA_H
#define CAMERA_H

#include <glm\glm.hpp>
class Camera {
public:
	Camera();
	void initialize( int gridX, int gridY, int gridZ );
	void updateCamera();
	glm::mat4 getLookAtMatrix();
	glm::vec3 getViewDirection();
	inline glm::vec3 getEyeLocation() { return v4tov3(cameraPoint); };
private:
	glm::vec4 cameraPoint;
	glm::vec4 lookAtPoint;
	glm::vec4 upAxis;

	int prevMouseX;
	int prevMouseY;
	int prevWheel;
	bool prevMousePressed;

	glm::vec3 v4tov3( glm::vec4 vector );
	glm::vec4 v3tov4( glm::vec3 vector );

	void rightButtonUpdates();
	void scrollWheelUpdates();
	void arrowUpdates();
};

#endif
