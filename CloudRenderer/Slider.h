#ifndef SLIDER_H
#define SLIDER_H

#include <string>

namespace slider_consts {
	const float sliderLength = 0.30f;
	const float sliderHeight = 0.008f;
	const float buttonSize = 0.03f;
	const float sliderOffsetY = 0.07f;
	const float sliderPositionX = 0.65f;
	const float sliderPositionY = 0.90f;
}

namespace {

	// Hidden in unnamed namespace for internal use
	struct Position {
		Position() {};
		Position( float x, float y ) : X(x), Y(y) {};
		float X;
		float Y;
	};

}

class Slider {
public:
	Slider( const std::string text, const std::string shaderProperty, 
		const float min, const float max, const float intial,
		const float sliderPositionY );
	void update();
	void render( const GLuint * textures );
	float getPercentage();
private:
	const std::string text;
	const std::string shaderProperty;
	const float min;
	const float max;
	float currentPercentage;
	const Position sliderPosition;
	Position buttonPosition;
	bool buttonPressed;
};

#endif
