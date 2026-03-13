#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "ui.h"
struct uiManager {
	std::vector<Button> buttons;
	std::vector<sf::Text> texts;
	std::unordered_map<int, int> textMap; // for easy access to specific texts like FPS display
	void initUI(sf::Font& font);
	void render(sf::RenderWindow& window);
	void handleEvent(const sf::Event& event, sf::RenderWindow& window);
	void addButton(Button& btn);
	void addText(sf::Text& text);
	void addText(sf::Text& text, int id);
	void update(std::string& fpsText, sf::RenderWindow& window);
};