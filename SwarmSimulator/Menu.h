#pragma once

#include <SFML/Graphics.hpp>
#include "Globals.h"

class Button
{
public:
	using Action = void (*)();

	Button(const sf::Vector2f& position, const sf::Vector2f& size, const std::string name, Action action);

	void draw(sf::RenderTarget& target) const;
	bool contains(const sf::Vector2f& coords) const;

	void setSelected(bool value = true);
protected:
	sf::RectangleShape m_rectangle;
	sf::Text m_text;
	Action m_action;
};


class Menu
{
public:
	enum class MenuPlacement { RIGHT };

	Menu(MenuPlacement menuPlacement);

	void addButton(const std::string& name, Button::Action action);
	void setSelected(int selected);

	void draw(sf::RenderTarget& target) const;
	bool contains(const sf::Vector2f& coords) const;

	const std::vector<Button>& getButtons() const;
protected:

protected:
	sf::RectangleShape m_rectangle;
	std::vector<Button> m_buttons;
	int m_selected;
};

