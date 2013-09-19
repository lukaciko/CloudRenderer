#include "stdafx.h"

#include "GUIControls.h"
#include "RenderUtility.h"

void GUIControls::addSlider( const std::string text, 
		const std::string shaderProperty, 
		const float min, const float max, const float initial ) {
	Slider slider = Slider( text, shaderProperty, min, max, initial, 
		sliders.size() * slider_consts::sliderOffsetY * -1 );
	sliders.push_back(slider);
	// Using a smart pointer would be better so copying is avoided
}

void GUIControls::update() {
	for( Slider &s : sliders )
		s.update();
}

void GUIControls::render( const GLuint * textures ) {
	float offset = 0;
	for ( Slider &s : sliders ) {
		s.render( textures );
	}
}