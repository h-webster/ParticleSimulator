#include <SFML/Graphics.hpp>
#include <vector>
#include "ui.h"
#include "uiManager.h"

void uiManager::addButton(Button& btn) {
	btn.shape.setFillColor(DEFAULT_BUTTON_COLOR);
	buttons.push_back(btn);

	// re-point sprite at the copied texture inside the vector
	if (buttons.back().contentType == ButtonContentType::Icon) {
		buttons.back().icon.emplace(*buttons.back().texture);
	}
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
	bool hoveringButton = false;
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

		//tooltip handling
		if (btn.contains(mousePos)) {
			if (!btn.tooltip.empty()) {
				tooltipText->setString(btn.tooltip);
				tooltipText->setPosition(mousePos + sf::Vector2f(10.f, 10.f));
				hoveringButton = true;
			}
		}
	}
	if (!hoveringButton) {
		tooltipText->setString("");
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
	//TODO store textures in a std::unordered_map<std::string, sf::Texture> keyed by filename

	// tooltip text
	tooltipText = new sf::Text(font);
	tooltipText->setCharacterSize(12);
	tooltipText->setFillColor(sf::Color::White);


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
	instructionsText.setString("C: Spawn 100 Particles");
	addText(instructionsText);

	// buttons

	Button spawnBtn(font, "Spawn", 14);
	spawnBtn.shape.setSize({ 60.f, 25.f });
	spawnBtn.tooltip = "Left click to spawn particles at mouse position";
	int idx = (int)buttons.size();
	spawnBtn.onClick = [this, idx]() {
		bool newState = !buttons[idx].isActive;
		buttons[idx].isActive = newState;  
	};
	addButton(spawnBtn, "spawn");

	textures.emplace_back();
	textures.back().loadFromFile("icons/gear.png");
	Button spawnSettingsBtn(textures.back());
	spawnSettingsBtn.shape.setSize({ 25.f, 25.f });
	spawnSettingsBtn.onClick = [this]() {
		events.push("spawnSettings");
	};
	addButton(spawnSettingsBtn, "spawnSettings");

	// Attract and repel buttons (MUST BE ADDED IN THIS ORDER FOR THE ONCLICK TO WORK PROPERLY)	
	Button attractBtn(font, "Attract", 14);
	attractBtn.shape.setSize({ 60.f, 25.f });
	attractBtn.tooltip = "Hold right click to attract particles to mouse";
	idx = (int)buttons.size();
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
	repelBtn.tooltip = "Hold right click to repel particles from mouse";
	idx = (int)buttons.size();
	repelBtn.onClick = [this, idx]() {
		bool newState = !buttons[idx].isActive;
		buttons[idx].isActive = newState;
		if(buttons[idx - 1].isActive) {
			buttons[idx - 1].isActive = false; // turn off repel if attract is turned on
		}
	};
	addButton(repelBtn, "repel");

	Button clearBtn(font, "Clear", 14);
	clearBtn.shape.setSize({ 60.f, 25.f });
	clearBtn.tooltip = "Clear all particles (space)";
	idx = (int)buttons.size();
	clearBtn.onClick = [this]() {
		events.push("clear");
	};
	addButton(clearBtn, "clear");

	float startX = 400.f;
	for (auto& btn : buttons) {
		float x;
		if (btn.contentType == ButtonContentType::Text) {
			x = startX + 80.f;
			startX += 80.f;
		}
		else {
			x = startX + 40.f;
			startX += 40.f;
		}
		btn.shape.setPosition({ x, 10.f });
		btn.alignContent();
	}

}
void uiManager::render(sf::RenderWindow& window) {
	for (auto& btn : buttons) {
		window.draw(btn.shape);
		if (btn.contentType == ButtonContentType::Text) {
			window.draw(*btn.label);
		}
		else {
			window.draw(*btn.icon);
		}
	}
	for (auto& text : texts) {
		window.draw(text);
	}
	window.draw(*tooltipText);
}
bool uiManager::pollEvent(std::string& out) {
	if (events.empty()) return false;
	out = events.front();
	events.pop();
	return true;
}