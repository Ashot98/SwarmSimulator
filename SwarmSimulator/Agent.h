#pragma once

#include "Entity.h"
#include <array>

class Predator;

class Agent : public Entity
{
public:
	using Agents = std::vector<Agent>;
	using Predators = std::vector<Predator>;
	using Obstacles = std::vector<sf::CircleShape>;
	using Goal = sf::CircleShape;

	Agent();

	void update(const Agents& agents, const Predators& predators, const Obstacles& obstacles, const Goal& goal, float dt);

	sf::Vector2f getVelocity() const;
	sf::VertexArray getVelocityDirection() const;
	sf::VertexArray getForcesDirection() const;
	sf::CircleShape getNeighborRadius() const;
protected:
	void boidsAdjustments(const Agents& agents, const Predators& predators, const Obstacles& obstacles, const Goal& goal, float dt);
	void calculateSeparationForce(const Agents& agents);
	void calculateAlignmentForce(const Agents& agents);
	void calculateCohesionForce(const Agents& agents);
	void calculateWallAvoidanceForce();
	void calculateObstacleAvoidanceForce(const Obstacles& obstacles);
	void calculateGoalAttractionForce(const Goal& goal);
	void calculatePredatorEscapeForce(const Predators& predators);

protected:
	std::array<sf::Vector2f, 7> m_forces;
	sf::CircleShape m_neighborRadius;
};