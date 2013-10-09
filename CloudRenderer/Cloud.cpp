#include "stdafx.h"

#include "Cloud.h"

Cloud::Cloud( const glm::vec3 position, const float size ):
	position(position), size(size) {}

void Cloud::move( const float horizontalFactor, const float hightFactor ) {
	// Moves the cloud. Higher clouds move faster.
	position.x += horizontalFactor * ( position.y + hightFactor );
}