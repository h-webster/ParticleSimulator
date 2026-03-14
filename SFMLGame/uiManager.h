#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "ui.h"
struct uiManager {
	std::vector<Button> buttons;
	std::vector<sf::Text> texts;
	std::unordered_map<std::string, int> textMap; // for easy access to specific texts like FPS display
	std::unordered_map<std::string, int> buttonMap; // for easy access to specific buttons by name
	void initUI(sf::Font& font);
	void render(sf::RenderWindow& window);
	void handleEvent(const sf::Event& event, sf::RenderWindow& window);
	void addButton(Button& btn);
	void addButton(Button& btn, std::string name);
	Button& getButton(std::string name);
	void addText(sf::Text& text);
	void addText(sf::Text& text, std::string name);
	sf::Text& getText(std::string name);
	void update(std::string& fpsText, sf::RenderWindow& window);
};