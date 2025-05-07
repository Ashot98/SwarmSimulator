#include "Globals.h"

sf::VideoMode				Globals::desktop;
sf::Font					Globals::font;
sf::Vector2f				Globals::fieldSize = { 800, 600 };
unsigned					Globals::area = 2e7; //m^2
float						Globals::minVelocity = 0.0f;
float						Globals::maxVelocity = 55.0f; //m/s
float						Globals::agentSize = 2.0f;
sf::Color					Globals::agentColor = sf::Color::Green;
sf::Color					Globals::predatorColor = sf::Color::Red;
size_t						Globals::agentCount = 50;
size_t						Globals::predatorCount = 3;
size_t						Globals::obstacleCount = 0;
bool						Globals::isGoalSet = false;
bool						Globals::isDebugEnabled = false;
PlacementType				Globals::placementType = PlacementType::NONE;
std::default_random_engine	Globals::rng = std::default_random_engine(std::random_device{}());
float						Globals::neighborRadius = 60.0f;
float						Globals::separationRadius = 20.0f;
float						Globals::separationStrength = 30.0f;
float						Globals::alignmentStrength = 100.0f;
float						Globals::cohesionStrength = 5.0f;
float						Globals::wallAvoidanceRadius = 50.f;
float						Globals::wallAvoidanceStrength = 700.f;
float						Globals::obstacleAvoidanceRadius = 50.f;
float						Globals::obstacleAvoidanceStrength = 3000.f;
float						Globals::goalAttractionStrength = 50.0f;
float						Globals::predatorEscapeRadius = 50.f;
float						Globals::predatorEscapeStrength = 3000.f;
