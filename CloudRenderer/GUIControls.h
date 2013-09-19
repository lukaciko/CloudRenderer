// Contains controls such as sliders

#ifndef CONTROLS_H
#define CONTROLS_H

#include <vector>
#include "Slider.h"

class GUIControls {
public:
	void update();
	void render( const GLuint * textures );
	void addSlider( const std::string text, const std::string shaderProperty, 
		const float min, const float max, const float initial );
private:
	std::vector<Slider> sliders;
};

#endif