#pragma once
#include <SFML/System/Vector2.hpp>


//Interpolate
sf::Vector2f lerp(const sf::Vector2f& a, const sf::Vector2f& b, const float factor);

sf::Vector2f cLerp(const sf::Vector2f& a, const sf::Vector2f b, const float step);