#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

// Constants shared across files
const int WIDTH = 1500;
const int HEIGHT = 800;
const float GRAVITY = 500.0f;
const float RESTITUTION = 0.9f;
const float DAMPING = 0.999f;
const float PARTICLE_RADIUS = 4.0f;

extern int cols, rows, cellSize;

struct Particle {
    sf::Vector2f pos;
    sf::Vector2f vel;
    sf::Vector2i cell;
    int key;
    float radius;
    float mass;
    sf::Color color;

    Particle(float x, float y, float r);

    void updateCell();
    void update(float dt);
    void collide(Particle& other);
    
};
void spawnParticles(std::vector<Particle>& particles, std::vector<std::vector<int>>& grid, int amount);
void renderAll(sf::RenderWindow& window, const std::vector<Particle>& particles);