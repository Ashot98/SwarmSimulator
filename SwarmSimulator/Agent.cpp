#include <random>
#include <cmath>
#include <iostream>
#include "Agent.h"
#include "Predator.h"
#include "Globals.h"
#include "Utils.h"

Agent::Agent()
	: Entity(Globals::agentColor) 
{
	m_neighborRadius.setFillColor(sf::Color::Transparent);
	m_neighborRadius.setOutlineColor(sf::Color::Green);
	m_neighborRadius.setOutlineThickness(1.f);
	m_neighborRadius.setRadius(Globals::neighborRadius);
	m_neighborRadius.setOrigin(Globals::neighborRadius, Globals::neighborRadius); //shifting origin, to set corrtly to center of agent
}

void Agent::update(const Agents& agents, const Predators& predators, const Obstacles& obstacles, const Goal& goal, float dt)
{
	boidsAdjustments(agents, predators, obstacles, goal, dt);
	move(dt);
	m_neighborRadius.setPosition(getPosition());
}

sf::Vector2f Agent::getVelocity() const
{
	return m_velocity;
}

sf::VertexArray Agent::getVelocityDirection() const
{
	sf::VertexArray line(sf::Lines, 2);
	line[0].position = getPosition();
	line[0].color = sf::Color::Green;
	line[1].position = getPosition() + normalize(m_velocity) * (length(m_velocity) * 10  / Globals::maxVelocity);
	line[1].color = sf::Color::Green;

	return line;
}

sf::VertexArray Agent::getForcesDirection() const
{
	sf::VertexArray lines(sf::Lines, m_forces.size()*2);
	const auto& origin = getPosition();

	for (size_t i = 0; i < m_forces.size(); ++i) {
		lines[i*2].position = origin;
		lines[i * 2 + 1].position = origin + m_forces[i] * 10.0f;

		lines[i * 2].color = sf::Color::Magenta;
		lines[i * 2 + 1].color = sf::Color::Magenta;
	}

	return lines;
}

sf::CircleShape Agent::getNeighborRadius() const
{
	return m_neighborRadius;
}

void Agent::boidsAdjustments(const Agents& agents, const Predators& predators, const Obstacles& obstacles, const Goal& goal, float dt)
{
	calculateSeparationForce(agents);
	calculateAlignmentForce(agents);
	calculateCohesionForce(agents);
	calculateWallAvoidanceForce();
	calculateObstacleAvoidanceForce(obstacles);
	calculateGoalAttractionForce(goal);
	calculatePredatorEscapeForce(predators);


	const auto& acceleration = (Globals::separationStrength * m_forces[0] +
		Globals::alignmentStrength * m_forces[1] +
		Globals::cohesionStrength * m_forces[2] +
		Globals::wallAvoidanceStrength * m_forces[3] +
		Globals::obstacleAvoidanceStrength * m_forces[4] +
		Globals::goalAttractionStrength * m_forces[5] +
		Globals::predatorEscapeStrength * m_forces[6]) * dt;
	m_velocity += acceleration;
	clampVelocity(m_velocity);
}

void Agent::calculateSeparationForce(const Agents& agents)
{
	m_forces[0] = {0.f, 0.f};
	int count = 0;

	for (const auto& agent : agents) {
		if (&agent == this) continue;

		const auto diff = getPosition() - agent.getPosition();
		const auto distance = std::sqrt(diff.x * diff.x + diff.y * diff.y);


		if (distance < Globals::separationRadius && distance > 0.001f) {
			m_forces[0] += diff * 1.0f / distance;
			++count;
		}
	}

	if (count > 0) {
		m_forces[0] /= static_cast<float>(count);
		m_forces[0] = normalize(m_forces[0]);
		m_forces[0] -= normalize(m_velocity);
	}
}

void Agent::calculateAlignmentForce(const Agents& agents)
{
	m_forces[1] = { 0.f, 0.f };
	int count = 0;

	for (const auto& agent : agents) {
		if (&agent == this) continue;

		const auto diff = getPosition() - agent.getPosition();
		const auto distance = std::sqrt(diff.x * diff.x + diff.y * diff.y);


		if (distance < Globals::neighborRadius) {
			m_forces[1] += normalize(agent.getVelocity());
			++count;
		}
	}

	if (count > 0) {
		m_forces[1] /= static_cast<float>(count);
		m_forces[1] = normalize(m_forces[1]);
	}
}

void Agent::calculateCohesionForce(const Agents& agents)
{
	m_forces[2] = { 0.f, 0.f };
	int count = 0;

	for (const auto& agent : agents) {
		if (&agent == this) continue;

		const auto diff = getPosition() - agent.getPosition();
		const auto distance = std::sqrt(diff.x * diff.x + diff.y * diff.y);


		if (distance < Globals::neighborRadius) {
			m_forces[2] += agent.getPosition();
			++count;
		}
	}

	if (count > 0) {
		m_forces[2] /= static_cast<float>(count);
		m_forces[2] -= getPosition();
		m_forces[2] = normalize(m_forces[2]);
	}
}

void Agent::calculateWallAvoidanceForce()
{
	m_forces[3] = { 0.f, 0.f };

	const auto& currPosition = getPosition();
	if (currPosition.x < Globals::wallAvoidanceRadius) {
		m_forces[3].x += std::pow(1.0f - currPosition.x / Globals::wallAvoidanceRadius, 2);
	}
	else if (Globals::fieldSize.x - currPosition.x < Globals::wallAvoidanceRadius) {
		m_forces[3].x -= std::pow(1.0f - (Globals::fieldSize.x - currPosition.x) / Globals::wallAvoidanceRadius, 2);
	}

	if (currPosition.y < Globals::wallAvoidanceRadius) {
		m_forces[3].y += std::pow(1.0f - currPosition.y / Globals::wallAvoidanceRadius, 2);
	}
	else if (Globals::fieldSize.y - currPosition.y < Globals::wallAvoidanceRadius) {
		m_forces[3].y -= std::pow(1.0f - (Globals::fieldSize.y - currPosition.y) / Globals::wallAvoidanceRadius, 2);
	}

	if (length(m_forces[3]) > 0.0f) {
		m_forces[3] = normalize(m_forces[3]);
	}
}

void Agent::calculateObstacleAvoidanceForce(const Obstacles& obstacles)
{
	m_forces[4] = { 0.f, 0.f };

	for (const auto& obstacle : obstacles) {
		const auto diff = getPosition() - obstacle.getPosition();
		const auto distance = std::sqrt(diff.x * diff.x + diff.y * diff.y) - obstacle.getRadius();

		if (distance < Globals::obstacleAvoidanceRadius) {
			const auto dir = normalize(diff);
			const auto falloff = (Globals::obstacleAvoidanceRadius - distance) / Globals::obstacleAvoidanceRadius;
			m_forces[4] += dir * falloff * falloff;
		}
	}
}

void Agent::calculateGoalAttractionForce(const Goal& goal)
{
	m_forces[5] = Globals::isGoalSet ? normalize(goal.getPosition() - getPosition()) : sf::Vector2f(0,0);
}

void Agent::calculatePredatorEscapeForce(const Predators& predators)
{
	m_forces[6] = { 0.f, 0.f };

	for (const auto& predator : predators) {
		const auto diff = getPosition() - predator.getPosition();
		const auto distance = std::sqrt(diff.x * diff.x + diff.y * diff.y);

		if (distance < Globals::predatorEscapeRadius) {
			const auto dir = normalize(diff);
			const auto falloff = (Globals::predatorEscapeRadius - distance) / Globals::predatorEscapeRadius;
			m_forces[6] += dir * falloff * falloff;
		}
	}
}
