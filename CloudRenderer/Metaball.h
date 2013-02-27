#ifndef METABALL_H
#define METABALL_H

#include <glm\glm.hpp>

class Metaball {
public:
	Metaball( glm::vec3 );
	glm::vec3 getPosition();
	void setColor( float );
	float getColor();
	void setDensity( float );
	float getDensity();
private:
	const glm::vec3 position;
	float color;
	float density;
};

#endif