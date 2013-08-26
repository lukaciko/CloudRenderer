#include "stdafx.h"

#include <iostream>

#include "Slider.h"
#include "RenderUtility.h"

Slider::Slider( std::string text, std::string shaderProperty, float min,
			   float max, float sliderPositionY ) : 
	text( text ),
	shaderProperty( shaderProperty ),
	min( min ),
	max( max ),
	current( ( max + min ) / 2 ),
	sliderPosition( slider_consts::sliderPositionX, 
		slider_consts::sliderPositionY + sliderPositionY ),
	buttonPosition( sliderPosition ) {}

void Slider::update() {
	
	int newMouseX, newMouseY;
	glfwGetMousePos( &newMouseX, &newMouseY );			

	float relativeMouseX = convertXToRelative( newMouseX );
	float relativeMouseY = convertYToRelative( newMouseY );

	std::cout << "X: " << relativeMouseX << " Y: " << relativeMouseY << "\n";

	glfwGetMouseButton(0);
}

void Slider::render() {
	setUniform( "offsetX", sliderPosition.X ) ;//getPercentage() * slider_consts::sliderLength );
	setUniform( "offsetY", buttonPosition.Y );
	glDrawElements( GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0 );
	
}

float Slider::getPercentage() {
	return ( current - min ) / ( max - min );
}