#include "ui.h"
#include <SFML/Graphics.hpp>
Button::Button(sf::Font& font, const std::string& text, int fontSize) : 
    isActive(false), contentType(ButtonContentType::Text) 
{
    label.emplace(font, text, fontSize);
}
Button::Button(sf::Texture& tex) : 
    isActive(false), contentType(ButtonContentType::Icon)
{
	texture.emplace(tex);
    icon.emplace(*texture);
}
bool Button::contains(sf::Vector2f point) {
	return shape.getGlobalBounds().contains(point);
}
void Button::alignContent() {
    if (contentType == ButtonContentType::Text) {
        sf::FloatRect textBounds = label->getLocalBounds();
        label->setOrigin({
            textBounds.position.x + textBounds.size.x / 2.f,
            textBounds.position.y + textBounds.size.y / 2.f
            });
        // Set position to center of the button shape
        label->setPosition({
            shape.getPosition().x + shape.getSize().x / 2.f,
            shape.getPosition().y + shape.getSize().y / 2.f
            });
    }
    else {
        sf::FloatRect bounds = shape.getGlobalBounds();
        icon->setPosition(bounds.position);
        auto texSize = texture->getSize();
        icon->setScale({
            bounds.size.x / texSize.x,
            bounds.size.y / texSize.y
            });
    }
}
void Button::highlight() {
    shape.setFillColor(HIGHLIGHT_BUTTON_COLOR);
}
void Button::normal() {
    shape.setFillColor(DEFAULT_BUTTON_COLOR);
}

void Dropdown::display() {

}
