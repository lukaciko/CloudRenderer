#include "stdafx.h"

#include "GUIControls.h"

void GUIControls::addSlider( std::string text, std::string shaderProperty, 
		float min, float max ) {
	Slider slider = Slider( text, shaderProperty, min, max );
	sliders.push_back(slider);
	// Using a smart pointer would be better so copying is avoided
}

void GUIControls::update() {
	for( Slider &s : sliders )
		s.updateSlider();
}

void GUIControls::render() {
	int offset = 0;
	for ( Slider &s : sliders ) {

		

		offset += 20;
	}

}