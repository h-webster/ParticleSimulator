#pragma once
#include <SFML/Graphics.hpp>
#include <functional>
const sf::Color DEFAULT_BUTTON_COLOR(120, 120, 120);
const sf::Color HIGHLIGHT_BUTTON_COLOR(150, 150, 150);
const sf::Color SELECTED_BUTTON_COLOR(99, 126, 194);

enum class ButtonContentType { Text, Icon };
struct Button {
	sf::RectangleShape shape;
	std::optional<sf::Text> label;
	std::optional<sf::Sprite> icon;
	std::optional<sf::Texture> texture;
	ButtonContentType contentType;
	std::string tooltip;
	bool isActive;
	std::function<void()> onClick;
	Button(sf::Font& font, const std::string& text, int fontSize);
	Button(sf::Texture& tex);

	bool contains(sf::Vector2f point);
	void highlight();
	void normal();
	void alignContent();
};
struct Dropdown {
	std::vector<Button> options;
	bool isOpen;
	void display();
};
/*
struct Menu {
	sf::Text title;
	std::vector<Button> buttons;
	bool isOpen;
	Menu(std::string);
}*/