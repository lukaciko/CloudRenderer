#ifndef SLIDER_H
#define SLIDER_H

#include <string>

class Slider {
public:
	Slider( std::string text, std::string shaderProperty, 
		float min, float max );
	void updateSlider();
private:
	const std::string text;
	const std::string shaderProperty;

	const float min;
	const float max;
	float current;
};

#endif
