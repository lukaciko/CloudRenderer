// Contains controls such as sliders

#ifndef CONTROLS_H
#define CONTROLS_H

#include <vector>
#include "Slider.h"

class GUIControls {
public:
	void update();
	void render();
	void addSlider( std::string text, std::string shaderProperty, 
		float min, float max );
private:
	std::vector<Slider> sliders;
};

#endif