#pragma once

#include <random>
#include <SFML/Graphics.hpp>

enum class PlacementType {
	NONE,
	OBSTACLE,
	GOAL
};

class Globals {
public:
	static sf::VideoMode desktop;
	static sf::Font font;
	static sf::Vector2f fieldSize;
	static unsigned area;
	static float minVelocity;
	static float maxVelocity;
	static float agentSize;
	static sf::Color agentColor;
	static sf::Color predatorColor;
	static size_t agentCount;
	static size_t predatorCount;
	static size_t obstacleCount;
	static bool isGoalSet;
	static bool isDebugEnabled;
	static PlacementType placementType;
	static std::default_random_engine rng;
	static float neighborRadius;
	static float separationRadius;
	static float separationStrength;
	static float alignmentStrength;
	static float cohesionStrength;
	static float wallAvoidanceRadius;
	static float wallAvoidanceStrength;
	static float obstacleAvoidanceRadius;
	static float obstacleAvoidanceStrength;
	static float goalAttractionStrength;
	static float predatorEscapeRadius;
	static float predatorEscapeStrength;
};