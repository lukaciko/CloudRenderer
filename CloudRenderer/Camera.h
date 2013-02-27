// Takes care of camera manipulation

#ifndef CAMERA_H
#define CAMERA_H

#include <glm\glm.hpp>
class Camera {
public:
	Camera();
	void initialize( int, int, int );
	void updateCamera();
	glm::mat4 getLookAtMatrix();
private:
	glm::vec4 cameraPoint;
	glm::vec4 lookAtPoint;
	glm::vec4 upAxis;

	int prevMouseX;
	int prevMouseY;
	int prevWheel;
	bool prevMousePressed;

	glm::vec3 v4tov3( glm::vec4 );
	glm::vec4 v3tov4( glm::vec3 );

	void rightButtonUpdates();
	void scrollWheelUpdates();
	void arrowUpdates();
};

#endif
