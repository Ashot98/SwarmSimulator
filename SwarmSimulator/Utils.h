#pragma once

#include <cmath>
#include <SFML/Graphics.hpp>
#include "Globals.h"

inline void clampVelocity(sf::Vector2f& velocity) 
{
	float speed = std::sqrt(velocity.x * velocity.x + velocity.y * velocity.y);

	if (speed > Globals::maxVelocity) {
		velocity = velocity / speed * Globals::maxVelocity;
	}
	if (speed < Globals::minVelocity) {
		velocity = velocity / speed * Globals::minVelocity;
	}
}

inline float length(const sf::Vector2f& vec)
{
	return std::sqrt(vec.x * vec.x + vec.y * vec.y);
}

inline sf::Vector2f normalize(const sf::Vector2f& vec)
{
	const float len = length(vec);
	if (len == 0) return vec;
	return vec / len;
}

inline float dot(const sf::Vector2f& v1, const sf::Vector2f& v2)
{
	return std::sqrt((v1.x - v2.x) * (v1.x - v2.x) + (v1.y - v2.y) * (v1.y - v2.y));
}