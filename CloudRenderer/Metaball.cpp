#include "stdafx.h"

#include "Metaball.h"

Metaball::Metaball( const glm::vec3 position ): position(position) {}

glm::vec3 Metaball::getPosition() {
	return position;
}

void Metaball::setColor( float color ) {
	this->color = color;
}

float Metaball::getColor() {
	return color;
}

void Metaball::setDensity( float density ) {
	this->density = density;
}

float Metaball::getDensity() {
	return density;
}