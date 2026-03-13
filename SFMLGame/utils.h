#pragma once

#include <SFML/Graphics.hpp>

int randomInt(int min, int max, float num);
float randomFloat(float min, float max, float num);
float distance(const sf::Vector2f& a, const sf::Vector2f& b);
float magnitude(const sf::Vector2f& v);
sf::Vector2f setMagnitude(sf::Vector2f v, float mag);

const float PI = 3.14159265f;
