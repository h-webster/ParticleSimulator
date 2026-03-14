#include <SFML/Graphics.hpp>
#include <vector>
#include "ui.h"
#include "uiManager.h"

void uiManager::addButton(Button& btn) {
	btn.alignTextPosition();
	btn.shape.setFillColor(DEFAULT_BUTTON_COLOR);
	buttons.push_back(btn);
}
void uiManager::addButton(Button& btn, std::string name) {
	addButton(btn);
	buttonMap[name] = buttons.size() - 1;
}
Button& uiManager::getButton(std::string name) {
	return buttons[buttonMap[name]];
}

void uiManager::addText(sf::Text& text) {
	texts.push_back(text);
}
void uiManager::addText(sf::Text& text, std::string name) {
	addText(text);
	textMap[name] = texts.size()-1;
}
sf::Text& uiManager::getText(std::string name) {
	return texts[textMap[name]];
}

void uiManager::update(std::string& fpsText, sf::RenderWindow& window) {
	getText("fps").setString(fpsText); 

	sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
	for (auto& btn : buttons) {
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
	addText(fpsText, "fps");

	sf::Text instructionsText(font);
	instructionsText.setFont(font);
	instructionsText.setCharacterSize(14);
	instructionsText.setFillColor(sf::Color::White);
	instructionsText.setPosition({ 200.f, 10.f });
	instructionsText.setString("Left Click: Spawn Particle | Hold Right Click: Interact | Space: Clear | C: Spawn 100 Particles");
	addText(instructionsText);

	// buttons

	// Attract and repel buttons (MUST BE ADDED IN THIS ORDER FOR THE ONCLICK TO WORK PROPERLY)	
	Button attractBtn(font, "Attract", 14);
	attractBtn.shape.setSize({ 60.f, 25.f });
	attractBtn.shape.setPosition({ 800.f, 10.f });
	int idx = buttons.size();
	attractBtn.onClick = [this, idx]() {
		bool newState = !buttons[idx].isActive;
		buttons[idx].isActive = newState;
		if (buttons[idx + 1].isActive) {
			buttons[idx + 1].isActive = false; // turn off repel if attract is turned on
		}
	};
	addButton(attractBtn, "attract");

	Button repelBtn(font, "Repel", 14);
	repelBtn.shape.setSize({ 60.f, 25.f });
	repelBtn.shape.setPosition({ 880.f, 10.f });
	idx = buttons.size();
	repelBtn.onClick = [this, idx]() {
		bool newState = !buttons[idx].isActive;
		buttons[idx].isActive = newState;
		if(buttons[idx - 1].isActive) {
			buttons[idx - 1].isActive = false; // turn off repel if attract is turned on
		}
	};
	addButton(repelBtn, "repel");
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