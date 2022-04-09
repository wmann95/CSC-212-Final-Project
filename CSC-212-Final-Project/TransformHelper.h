#pragma once
#include <SFML/System/Vector2.hpp>

using namespace sf;

//Interpolate
Vector2f lerp(const Vector2f& a, const Vector2f& b, float factor) {
	if (factor < 0.0) {
		factor = 0.0;
	}
	else if (factor > 1.0) {
		factor = 1.0;
	}

	return a + (b - a) * factor;
}