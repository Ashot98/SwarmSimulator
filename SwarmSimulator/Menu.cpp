#include "Menu.h"

Button::Button(const sf::Vector2f& position, const sf::Vector2f& size, const std::string name, Action action)
	: m_action(action)
{
	m_rectangle.setSize(size);
	m_rectangle.setPosition(position);
	m_rectangle.setFillColor(sf::Color::White);

	m_text.setFont(Globals::font);
	m_text.setString(name);
	m_text.setCharacterSize(16);
	m_text.setFillColor(sf::Color::Black);
	const sf::FloatRect& textBounds = m_text.getLocalBounds();
	const sf::Vector2f& rectPos = m_rectangle.getPosition();
	const sf::Vector2f& rectSize = m_rectangle.getSize();

	// Center the text
	m_text.setOrigin(textBounds.left + textBounds.width / 2.0f,
		textBounds.top + textBounds.height / 2.0f);
	m_text.setPosition(rectPos.x + rectSize.x / 2.0f,
		rectPos.y + rectSize.y / 2.0f);
}

void Button::draw(sf::RenderTarget& target) const
{
	target.draw(m_rectangle);
	target.draw(m_text);
}

bool Button::contains(const sf::Vector2f& coords) const
{
	return m_rectangle.getGlobalBounds().contains(coords);
}

void Button::setSelected(bool value)
{
	if (value)
		m_rectangle.setFillColor(sf::Color::Color(90, 90, 90));
	else
		m_rectangle.setFillColor(sf::Color::White);

	m_action();
}

Menu::Menu(MenuPlacement menuPlacement)
	: m_selected(-1)
{
	sf::Vector2f position;
	switch (menuPlacement) {
	case MenuPlacement::RIGHT:
		position.x = static_cast<float>(Globals::desktop.width) - static_cast<float>(Globals::desktop.width) / 5;
		position.y = 0;
		break;
	default:
		throw std::runtime_error("Wrong menu placement type");
	}

	sf::Vector2f size{ static_cast<float>(Globals::desktop.width) / 5, static_cast<float>(Globals::desktop.height) };

	m_rectangle.setSize(size);
	m_rectangle.setPosition(position);
	m_rectangle.setFillColor(sf::Color::Color(155, 155, 155));
}

void Menu::addButton(const std::string& name, Button::Action action)
{
	sf::Vector2f position = m_rectangle.getPosition();
	const size_t buttonsCnt = m_buttons.size();
	position.y += buttonsCnt * 60 + (buttonsCnt + 1) * 10;
	position.x += 10;

	sf::Vector2f size{ m_rectangle.getSize().x - 20, 50 };

	m_buttons.emplace_back(position, size, name, action);
}

void Menu::setSelected(int selected)
{
	if(m_selected != -1)
		m_buttons[m_selected].setSelected(false);
	if (m_selected == selected) return;
	m_buttons[selected].setSelected(true);
	m_selected = selected;
}

void Menu::draw(sf::RenderTarget& target) const
{
	target.draw(m_rectangle);

	for (auto& button : m_buttons) {
		button.draw(target);
	}
}

bool Menu::contains(const sf::Vector2f& coords) const
{
	return m_rectangle.getGlobalBounds().contains(coords);
}

const std::vector<Button>& Menu::getButtons() const
{
	return m_buttons;
}
