#include "stdafx.h"

#include <iostream>

#include "Slider.h"
#include "RenderUtility.h"

Slider::Slider( const std::string text, const std::string shaderProperty, 
		const float min, const float max, const float initial,
		const float sliderPositionY ) : 
	text( text ),
	shaderProperty( shaderProperty ),
	min( min ),
	max( max ),
	currentPercentage( ( initial - min ) / ( max - min ) ),
	sliderPosition( slider_consts::sliderPositionX, 
		slider_consts::sliderPositionY + sliderPositionY ),
	buttonPosition( Position( sliderPosition.X + slider_consts::sliderLength * 
		(-0.5f + currentPercentage), sliderPosition.Y )),
	buttonPressed( false ) {
		setUniform( shaderProperty, min + currentPercentage * (max - min) );
	}

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
		setUniform( shaderProperty, min + currentPercentage * (max - min) );
	}

}

void Slider::render( const GLuint * textures ) {

	// Render the left part of the slider
	glBindTexture( GL_TEXTURE_2D, textures[1] );
	float minPos = sliderPosition.X - slider_consts::sliderLength / 2;
	setUniform( "positionX", ( minPos + buttonPosition.X ) / 2 );
	setUniform( "positionY", buttonPosition.Y );
	setUniform( "sizeX", ( buttonPosition.X - minPos ) / 2);
	setUniform( "sizeY", slider_consts::sliderHeight );
	glDrawElements( GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0 );

	// Render the right part of the slider
	glBindTexture( GL_TEXTURE_2D, textures[2] );
	float maxPos = sliderPosition.X + slider_consts::sliderLength / 2;
	setUniform( "positionX", ( maxPos + buttonPosition.X ) / 2 );
	setUniform( "positionY", buttonPosition.Y );
	setUniform( "sizeX", ( maxPos - buttonPosition.X ) / 2);
	setUniform( "sizeY", slider_consts::sliderHeight );
	glDrawElements( GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0 );

	// Render the handle
	glBindTexture( GL_TEXTURE_2D, textures[0] );
	setUniform( "positionX", buttonPosition.X );
	setUniform( "positionY", buttonPosition.Y );
	setUniform( "sizeX", slider_consts::buttonSize );
	setUniform( "sizeY", slider_consts::buttonSize );
	glDrawElements( GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0 );
	
}

float Slider::getPercentage() {
	return ( currentPercentage - min ) / ( max - min );
}