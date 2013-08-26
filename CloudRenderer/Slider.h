#ifndef SLIDER_H
#define SLIDER_H

#include <string>

namespace slider_consts {
	const float sliderLength = 0.30f;
	const float buttonSize = 0.05f;
	const float sliderOffsetY = 0.12f;
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
	Slider( std::string text, std::string shaderProperty, 
		float min, float max, float sliderPositionY );
	void update();
	void render();
	float getPercentage();
private:
	const std::string text;
	const std::string shaderProperty;
	const Position sliderPosition;
	Position buttonPosition;
	const float min;
	const float max;
	
	float currentPercentage;
	bool buttonPressed;
};

#endif
