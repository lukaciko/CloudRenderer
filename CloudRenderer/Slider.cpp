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
	currentPercentage( ( max + min ) / 2 ),
	sliderPosition( slider_consts::sliderPositionX, 
		slider_consts::sliderPositionY + sliderPositionY ),
	buttonPosition( sliderPosition ),
	buttonPressed( false ) {}

void Slider::update() {
	
	int newMouseX, newMouseY;
	glfwGetMousePos( &newMouseX, &newMouseY );			

	float relativeMouseX = convertXToRelative( newMouseX );
	float relativeMouseY = convertYToRelative( newMouseY );

	// Check if mouse press is on button
	if( !glfwGetMouseButton(0) ) {
		buttonPressed = false;
	}
	else if( ( relativeMouseX - buttonPosition.X ) * ( relativeMouseX - buttonPosition.X )
		+ (relativeMouseY - buttonPosition.Y ) * ( relativeMouseY - buttonPosition.Y )
		< slider_consts::buttonSize * slider_consts::buttonSize || buttonPressed ){
		buttonPressed = true;
		buttonPosition.X = relativeMouseX;
		float minPos = sliderPosition.X - slider_consts::sliderLength / 2;
		float maxPos = sliderPosition.X + slider_consts::sliderLength / 2;
		if( buttonPosition.X <  minPos )
			buttonPosition.X = minPos;
		if( buttonPosition.X > maxPos )
			buttonPosition.X = maxPos;
	
		currentPercentage = ( buttonPosition.X - minPos ) / ( maxPos - minPos );
		setUniform( shaderProperty, currentPercentage );
	}

}

void Slider::render() {
	setUniform( "offsetX", buttonPosition.X ) ;//getPercentage() * slider_consts::sliderLength );
	setUniform( "offsetY", buttonPosition.Y );
	glDrawElements( GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0 );
	
}

float Slider::getPercentage() {
	return ( currentPercentage - min ) / ( max - min );
}