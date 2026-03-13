#include <SFML/Graphics.hpp>
#include <vector>
#include "ui.h"
#include <iostream>
#include "uiManager.h"

void uiManager::addButton(Button& btn) {
	btn.alignTextPosition();
	btn.shape.setFillColor(DEFAULT_BUTTON_COLOR);
	buttons.push_back(btn);
}
void uiManager::addText(sf::Text& text) {
	texts.push_back(text);
}
void uiManager::addText(sf::Text& text, int id) {
	texts.push_back(text);
	textMap[id] = texts.size()-1;
}
void uiManager::update(std::string& fpsText, sf::RenderWindow& window) {
	texts[textMap[0]].setString(fpsText); // 0 is for fpsText

	sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
	for (auto& btn : buttons) {
		std::cout << "Button " << btn.label.getString().toAnsiString() << " isActive: " << btn.isActive << std::endl;
		if (btn.isActive) {
			btn.shape.setFillColor(SELECTED_BUTTON_COLOR);
		}
		else if (btn.contains(mousePos)) {
			btn.highlight();
		}
		else {
			btn.normal();
		}
	}
}
void uiManager::handleEvent(const sf::Event& event, sf::RenderWindow& window) {
	if (const auto* mousePressed = event.getIf<sf::Event::MouseButtonPressed>()) {
		if (mousePressed->button == sf::Mouse::Button::Left) {
			sf::Vector2f mousePos = window.mapPixelToCoords(
				{ mousePressed->position.x, mousePressed->position.y }
			);
			for (auto& btn : buttons) {
				if (btn.contains(mousePos)) {
					std::cout << "Button " << btn.label.getString().toAnsiString() << " clicked!" << std::endl;
					btn.onClick();
				}
			}
		}
	}
}
void uiManager::initUI(sf::Font& font) {
	// instructions text
	sf::Text fpsText(font);
	fpsText.setFont(font);
	fpsText.setCharacterSize(14);
	fpsText.setFillColor(sf::Color::White);
	fpsText.setPosition({ 10.f, 10.f });
	addText(fpsText, 0);

	sf::Text instructionsText(font);
	instructionsText.setFont(font);
	instructionsText.setCharacterSize(14);
	instructionsText.setFillColor(sf::Color::White);
	instructionsText.setPosition({ 200.f, 10.f });
	instructionsText.setString("Left Click: Spawn Particle | Right Click: Repel | Space: Clear | C: Spawn 100 Particles");
	addText(instructionsText);

	// buttons
	Button attractBtn(font, "Attract", 14);

	attractBtn.shape.setSize({ 60.f, 25.f });
	attractBtn.shape.setPosition({ 800.f, 10.f });
	int idx = buttons.size();
	attractBtn.onClick = [this, idx]() {
		buttons[idx].isActive = true;
	};
	addButton(attractBtn);
}
void uiManager::render(sf::RenderWindow& window) {
	for (auto& btn : buttons) {
		window.draw(btn.shape);
		window.draw(btn.label);
	}
	for (auto& text : texts) {
		window.draw(text);
	}
}