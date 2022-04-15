#include "TransformHelper.h"
#include <math.h>

sf::Vector2f lerp(const sf::Vector2f& a, const sf::Vector2f& b, float factor) {
	 if (factor < 0.0f) {
		 factor = 0.0f;
	 }
	 else if (factor > 1.0f) {
		 factor = 1.0;
	 }

	 sf::Vector2f delta = b - a;
	 float d = delta.x * delta.y + delta.x * delta.y;
	 float l2 = d * d;

	 if (l2 <= factor * factor) {
		 return b;
	 }
 

	 return a + (b - a) * factor / 500.0f;
}


sf::Vector2f cLerp(const sf::Vector2f& a, const sf::Vector2f b, const float step) {
	sf::Vector2f delta = b - a;
	float d = delta.x * delta.y + delta.x * delta.y;
	float l2 = d * d;

	if (l2 <= step * step) {
		return b;
	}

	sf::Vector2f dir = delta / (float)sqrt(l2);

	return a + step * dir;
}