#pragma once
#include <SFML/Graphics.hpp>
#include <functional>
const sf::Color DEFAULT_BUTTON_COLOR(120, 120, 120);
const sf::Color HIGHLIGHT_BUTTON_COLOR(150, 150, 150);
const sf::Color SELECTED_BUTTON_COLOR(99, 126, 194);
struct Button {
	sf::RectangleShape shape;
	sf::Text label;
	bool isActive;
	std::function<void()> onClick;
	Button(sf::Font& font, const std::string& text, int fontSize);

	bool contains(sf::Vector2f point);
	void highlight();
	void normal();
	void alignTextPosition();
};