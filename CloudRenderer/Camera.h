// Takes care of camera manipulation

#ifndef CAMERA_H
#define CAMERA_H

#include <glm\glm.hpp>
class Camera {
public:
	Camera();
	void initialize( const int gridX, const int gridY, const int gridZ );
	void updateCamera();
	glm::mat4 getLookAtMatrix() const;
	glm::vec3 getViewDirection() const;
	inline glm::vec3 getEyeLocation() { return v4tov3(cameraPoint); };
private:
	glm::vec4 cameraPoint;
	glm::vec4 lookAtPoint;
	glm::vec4 upAxis;

	int prevMouseX;
	int prevMouseY;
	int prevWheel;
	bool prevMousePressed;

	glm::vec3 v4tov3( const glm::vec4 vector ) const;
	glm::vec4 v3tov4( const glm::vec3 vector ) const;

	void rightButtonUpdates();
	void scrollWheelUpdates();
	void arrowUpdates();
};

#endif
