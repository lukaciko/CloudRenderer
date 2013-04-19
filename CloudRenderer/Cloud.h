// Cloud data, used in cloud simulation

#ifndef CLOUD_H
#define CLOUD_H

#include <glm\glm.hpp>
class Cloud {
public:
	Cloud( const glm::vec3 position, const float size );
	inline glm::vec3 getPosition() const { return position; };
	inline float getSize() const { return size; };
private:
	glm::vec3 position;
	float size;
};

#endif
