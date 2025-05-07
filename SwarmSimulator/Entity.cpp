#include "Entity.h"
#include "Globals.h"

Entity::Entity(const sf::Color& color)
	: sf::CircleShape(Globals::agentSize)
{
	setRandomInitialConds();
	setFillColor(color);
}

void Entity::setRandomInitialConds()
{
	constexpr float PI = 3.14159265f;
	std::uniform_real_distribution<float> distX(0, Globals::fieldSize.x);
	std::uniform_real_distribution<float> distY(0, Globals::fieldSize.y);
	std::uniform_real_distribution<float> angleDist(0, 2 * PI);

	const auto x = distX(Globals::rng);
	const auto y = distY(Globals::rng);
	setPosition(x, y);

	float angle = angleDist(Globals::rng);
	m_velocity = { std::cos(angle), std::sin(angle) };
	m_velocity *= Globals::maxVelocity;
}

void Entity::move(float dt)
{
	const auto& coords = getPosition();
	auto newX = coords.x + m_velocity.x * dt;
	auto newY = coords.y + m_velocity.y * dt;

	if (newX > Globals::fieldSize.x) {
		newX = Globals::fieldSize.x;
		m_velocity.x *= -1;
	}
	else if (newX < 0) {
		newX = 0;
		m_velocity.x *= -1;
	}

	if (newY > Globals::fieldSize.y) {
		newY = Globals::fieldSize.y;
		m_velocity.y *= -1;
	}
	else if (newY < 0) {
		newY = 0;
		m_velocity.y *= -1;
	}

	setPosition(newX, newY);
}
