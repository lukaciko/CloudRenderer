#include "stdafx.h"

#include "GUIControls.h"
#include "RenderUtility.h"

void GUIControls::addSlider( std::string text, std::string shaderProperty, 
		float min, float max ) {
	Slider slider = Slider( text, shaderProperty, min, max, 
		sliders.size() * slider_consts::sliderOffsetY * -1 );
	sliders.push_back(slider);
	// Using a smart pointer would be better so copying is avoided
}

void GUIControls::update() {
	for( Slider &s : sliders )
		s.update();
}

void GUIControls::render() {
	float offset = 0;
	for ( Slider &s : sliders ) {
		s.render();
	}
}