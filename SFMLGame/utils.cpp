
#include "utils.h"
#include <cmath>
#include <iostream>

int randomInt(int min, int max, float num) {
    return static_cast<int>(num * (max - min) + min);
}
float randomFloat(float min, float max, float num) {
    return num * (max - min) + min;
}
float distance(const sf::Vector2f& a, const sf::Vector2f& b) {
    return std::sqrt((b.x - a.x) * (b.x - a.x) + (b.y - a.y) * (b.y - a.y));
}
float magnitude(const sf::Vector2f& v) {
    return std::sqrt(v.x * v.x + v.y * v.y);
}
sf::Vector2f setMagnitude(sf::Vector2f v, float mag) {
    float len = magnitude(v);
    if (len == 0.f) return v;  // avoid divide by zero
    return v / len * mag;
}

