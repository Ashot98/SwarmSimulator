#pragma once

#include <SFML/Graphics.hpp>

class Entity : public sf::CircleShape {
public:
	Entity(const sf::Color& color);

protected:
	void setRandomInitialConds();
	void move(float dt);

protected:
	sf::Vector2f m_velocity;
};

