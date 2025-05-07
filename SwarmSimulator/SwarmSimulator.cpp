#include <iostream>
#include <filesystem>
#include <SFML/Graphics.hpp>
#include "Agent.h"
#include "Predator.h"
#include "Globals.h"
#include "Menu.h"

using Agents = std::vector<Agent>;
using Predators = std::vector<Predator>;
using Obstacles = std::vector<sf::CircleShape>;
using Goal = sf::CircleShape;

Menu createMenu()
{
    Menu menu(Menu::MenuPlacement::RIGHT);
    menu.addButton("Obstacle", []() {
        if (Globals::placementType == PlacementType::OBSTACLE) {
            Globals::placementType = PlacementType::NONE;
        }
        else {
            Globals::placementType = PlacementType::OBSTACLE;
        }
        });
    menu.addButton("Goal", []() {
        if (Globals::placementType == PlacementType::GOAL) {
            Globals::placementType = PlacementType::NONE;
        }
        else {
            Globals::placementType = PlacementType::GOAL;
        }
        });

    return menu;
}

void updateMenu(Menu& menu, const sf::Vector2f& worldPos)
{
    const auto& buttons = menu.getButtons();

    for (size_t i = 0; i < buttons.size(); ++i) {
        if (buttons[i].contains(worldPos)) {
            menu.setSelected(i);
            break;
        }
    }
}

void addObstacle(sf::RenderWindow& window, const sf::Event& event, Obstacles& obstacles)
{
    const sf::Vector2i pixelPos(event.mouseButton.x, event.mouseButton.y);
    const sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);

    obstacles.emplace_back(30);
    obstacles.back().setPosition(worldPos);
    obstacles.back().setFillColor(sf::Color::Red);
    ++Globals::obstacleCount;
}

void updateGoal(sf::RenderWindow& window, const sf::Event& event, Goal& goal)
{
    const sf::Vector2i pixelPos(event.mouseButton.x, event.mouseButton.y);
    const sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);

    Globals::isGoalSet = true;
    goal.setRadius(4);
    goal.setPosition(worldPos);
    goal.setFillColor(sf::Color::Blue);
}

void handleEvents(sf::RenderWindow& window, Menu& menu, Obstacles& obstacles, Goal& goal)
{
    sf::Event event;
    while (window.pollEvent(event))
        if (event.type == sf::Event::Closed)
            window.close();
        else if (event.type == sf::Event::MouseButtonPressed) {
            const sf::Vector2i pixelPos(event.mouseButton.x, event.mouseButton.y);
            const sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);
            if (menu.contains(worldPos)) {
                updateMenu(menu, worldPos);
            }
            else if (event.mouseButton.button == sf::Mouse::Left) {
                if (Globals::placementType == PlacementType::OBSTACLE) {
                    addObstacle(window, event, obstacles);
                }
                else if (Globals::placementType == PlacementType::GOAL) {
                    updateGoal(window, event, goal);
                }
            }
        }
        else if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Q) {
                window.close();
            }
            if (event.key.code == sf::Keyboard::D) {
                Globals::isDebugEnabled = !Globals::isDebugEnabled;
            }
        }
}

void updateScene(sf::RenderWindow& window, sf::Clock& clk, Menu& menu, Agents& agents, Predators& predators, Obstacles& obstacles, const Goal& goal)
{
    const float dt = clk.restart().asSeconds();
    window.clear();
    menu.draw(window);
    for (size_t i = 0; i < Globals::agentCount; ++i) {
        agents[i].update(agents, predators, obstacles, goal, dt);
        window.draw(agents[i]);
        if (Globals::isDebugEnabled) {
            window.draw(agents[i].getNeighborRadius());
            window.draw(agents[i].getVelocityDirection());
            window.draw(agents[i].getForcesDirection());
        }
    }
    for (size_t i = 0; i < Globals::predatorCount; ++i) {
        predators[i].update(dt);
        window.draw(predators[i]);
    }
    for (size_t i = 0; i < Globals::obstacleCount; ++i) {
        window.draw(obstacles[i]);
    }
    if (Globals::isGoalSet) {
        window.draw(goal);
    }
    window.display();
}

int main() {
    Globals::desktop = sf::VideoMode::getDesktopMode();
    Globals::fieldSize.x = static_cast<float>(Globals::desktop.width) - static_cast<float>(Globals::desktop.width) / 5;
    Globals::fieldSize.y = static_cast<float>(Globals::desktop.height);
    sf::RenderWindow window(Globals::desktop, "Swarm Simulator", sf::Style::Fullscreen);

    if (!Globals::font.loadFromFile("Fonts\\ARIAL.TTF")) {
        std::cerr << "Failed to load font" << std::endl;
        return 1;
    }
    
    Menu menu = createMenu();

    Agents agents;
    for (size_t i = 0; i < Globals::agentCount; ++i) {
        agents.emplace_back(Agent());
    }

    Predators predators;
    for (size_t i = 0; i < Globals::predatorCount; ++i) {
        predators.emplace_back(Predator());
    }

    Obstacles obstacles;
    Goal goal;

    sf::Clock clk;
    while (window.isOpen()) {
        handleEvents(window, menu, obstacles, goal);
        updateScene(window, clk, menu, agents, predators, obstacles, goal);
    }

    return 0;
}