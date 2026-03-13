#include "ui.h"
#include <SFML/Graphics.hpp>
Button::Button(sf::Font& font, const std::string& text, int fontSize) : label(font, text, fontSize), isActive(false) {}
bool Button::contains(sf::Vector2f point) {
	return shape.getGlobalBounds().contains(point);
}
void Button::alignTextPosition() {
    sf::FloatRect textBounds = label.getLocalBounds();
    label.setOrigin({
        textBounds.position.x + textBounds.size.x / 2.f,
        textBounds.position.y + textBounds.size.y / 2.f
        });
    // Set position to center of the button shape
    label.setPosition({
        shape.getPosition().x + shape.getSize().x / 2.f,
        shape.getPosition().y + shape.getSize().y / 2.f
        });
}
void Button::highlight() {
    shape.setFillColor(HIGHLIGHT_BUTTON_COLOR);
}
void Button::normal() {
    shape.setFillColor(DEFAULT_BUTTON_COLOR);
}
