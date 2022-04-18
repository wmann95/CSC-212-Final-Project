#include "TransformHelper.h"
#include <math.h>
#include <cmath>

sf::Vector2f lerp(const sf::Vector2f& a, const sf::Vector2f& b, float factor) {
	 if (factor < 0.0f) {
		 factor = 0.0f;
	 }
	 else if (factor > 1.0f) {
		 factor = 1.0;
	 }


	 return a + (b - a) * factor / 500.0f;
}
