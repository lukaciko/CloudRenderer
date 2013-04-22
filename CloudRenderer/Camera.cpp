#include "stdafx.h"

#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>

Camera::Camera():

	cameraPoint( glm::vec4( 0.0f, 0.0f, 0.0f, 1.0f ) ),
	lookAtPoint( glm::vec4( 0.0f, 0.0f, 0.0f, 1.0f ) ),
	upAxis( glm::vec4( 0.0f, 1.0f, 0.0f, 0.0f ) ),

	prevWheel( 0 ),
	prevMousePressed( false ) {}

void Camera::initialize( const int gridX, const int gridY, const int gridZ ) {

	// Look vector located at the middle of the grid
	cameraPoint.x += 0.5;
	cameraPoint.y += 0.5;
	cameraPoint.z += -0.5;

	lookAtPoint.x += 0.5;
	lookAtPoint.y += 0.5;
	lookAtPoint.z += 0.5;

}

glm::mat4 Camera::getLookAtMatrix() const {

	return glm::lookAt(
		v4tov3( cameraPoint ),
		v4tov3( lookAtPoint ),
		v4tov3( upAxis )
		);

}

glm::vec3 Camera::getViewDirection() const {
	return v4tov3( glm::normalize( cameraPoint - lookAtPoint ) );
}

void Camera::updateCamera() {

	rightButtonUpdates();
	scrollWheelUpdates();
	arrowUpdates();

}

// Rotations with right mouse button
void Camera::rightButtonUpdates() {
	float rotationFactor = 0.2f;
	if( !glfwGetMouseButton(1) ) {
		if( prevMousePressed ) {
			// Right mouse button has been pressed for more than 1 frame
			int newMouseX, newMouseY;
			glfwGetMousePos( &newMouseX, &newMouseY );				
			
			int diffX = newMouseX - prevMouseX + 1;
			int diffY = newMouseY - prevMouseY;
			
			glm::vec3 transVec = v4tov3( lookAtPoint );
			glm::mat4 translateMatrix = 
					glm::translate( glm::mat4(), -transVec );
			glm::mat4 minusTranslateMatrix = 
					glm::translate( glm::mat4(), transVec );

			// Difference in X - rotate around up vector in lookAt point
			if( diffX ) {
				glm::vec3 rotAxis = v4tov3( upAxis );
				glm::mat4 rotMatrix = 
					glm::rotate( glm::mat4(), diffX*rotationFactor, rotAxis );
				cameraPoint = minusTranslateMatrix * rotMatrix 
					* translateMatrix * cameraPoint;
			}

			// Difference in Y - rotate around DxY vector in lookAt point
			// (D is a vector between camera and look-at point)
			if( diffY ) {
				glm::vec3 rotAxis = glm::cross( 
					v4tov3( upAxis ),
					v4tov3( cameraPoint - lookAtPoint ));
				glm::mat4 rotMatrix = 
					glm::rotate( glm::mat4(), diffY*rotationFactor, rotAxis );
				cameraPoint = minusTranslateMatrix * rotMatrix 
					* translateMatrix * cameraPoint;
			}

		}
		
		prevMousePressed = true;
		glfwGetMousePos( &prevMouseX, &prevMouseY );
		
	}
	else
		prevMousePressed = false;
}

// Translation with scroll wheel
void Camera::scrollWheelUpdates() {
	float scrollFactor = -0.03f;
	// Limit the distance of cameraPoint and lookAtPoint
	float minDSize = 0.25f;
	int newWheel = glfwGetMouseWheel();
	int wheelDiff = newWheel - prevWheel;
	if( wheelDiff ) {
		glm::vec4 d = cameraPoint - lookAtPoint;
		d *= (1 + wheelDiff*scrollFactor);
		if( glm::length( d ) > minDSize ) 
			cameraPoint = lookAtPoint + d;
	}
	prevWheel = newWheel;
}

void Camera::arrowUpdates() {

	float arrowFactor = 2.0f;
	glm::vec4 normD = glm::normalize( cameraPoint - lookAtPoint );
	glm::vec4 normDY = glm::normalize( v3tov4(
		glm::cross( v4tov3( normD ), v4tov3(upAxis) )));
	if( glfwGetKey( GLFW_KEY_UP ) == GLFW_PRESS ||
		glfwGetKey( 'W' ) == GLFW_PRESS ) {
			cameraPoint -= normD * arrowFactor;
			lookAtPoint -= normD * arrowFactor;
	}
	if( glfwGetKey( GLFW_KEY_DOWN ) == GLFW_PRESS ||
		glfwGetKey( 'S' ) == GLFW_PRESS ) {
			cameraPoint += normD * arrowFactor;
			lookAtPoint += normD * arrowFactor;
	}
	if( glfwGetKey( GLFW_KEY_LEFT ) == GLFW_PRESS ||
		glfwGetKey( 'A' ) == GLFW_PRESS ) {
			cameraPoint += normDY * arrowFactor;
			lookAtPoint += normDY * arrowFactor;
	}
	if( glfwGetKey( GLFW_KEY_RIGHT ) == GLFW_PRESS ||
		glfwGetKey( 'D' ) == GLFW_PRESS ) {
			cameraPoint -= normDY * arrowFactor;
			lookAtPoint -= normDY * arrowFactor;
	}

	// We might have changed w components with vector additions and 
	// subtractions. Reset w to 1
	cameraPoint.w = 1.0f;
	lookAtPoint.w = 1.0f;
}

glm::vec3 Camera::v4tov3( const glm::vec4& v1 ) const {
    return glm::vec3(v1.x, v1.y, v1.z);
}

glm::vec4 Camera::v3tov4( const glm::vec3& v1 ) const {
    return glm::vec4(v1.x, v1.y, v1.z, 1.0f);
}