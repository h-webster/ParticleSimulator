#include "particle.h"
#include "utils.h"
#include <vector>
#include <random>
#include <cmath>
#include <iostream>

int cols = (int)(WIDTH / (PARTICLE_RADIUS * 2));
int rows = (int)(HEIGHT / (PARTICLE_RADIUS * 2));
int cellSize = (int)(PARTICLE_RADIUS * 2);

Particle::Particle(float x, float y, float r) : pos(x, y), radius(r) {
    static std::mt19937 rng(std::random_device{}());
    static std::uniform_real_distribution<float> dist(0.f, 1.f);
    vel = sf::Vector2f(randomFloat(-100.f, 100.f, dist(rng)), randomFloat(-100.f, 100.f, dist(rng)));
    color = sf::Color(randomInt(0, 255, dist(rng)), randomInt(0, 255, dist(rng)), randomInt(0, 255, dist(rng)));
    updateCell();
    mass = sqrt(radius) * 20;
}

void Particle::updateCell() {
    int cellX = (int)(pos.x / cellSize);
    int cellY = (int)(pos.y / cellSize);
    cell = sf::Vector2i(cellX, cellY);
    key = cellX + cellY * cols;
}

void Particle::update(float dt) {
    pos += vel * dt;
	vel.y += GRAVITY * dt;
	vel *= DAMPING;

    if (pos.x < 0 || pos.x + radius * 2 > WIDTH) {
        vel.x *= -1 * RESTITUTION;
    }
    if (pos.y < 0 || pos.y + radius * 2 > HEIGHT) {
        vel.y *= -1 * RESTITUTION;
    }

    pos.x = std::clamp(pos.x, 0.f, static_cast<float>(WIDTH) - radius * 2);
    pos.y = std::clamp(pos.y, 0.f, static_cast<float>(HEIGHT) - radius * 2);
}

void Particle::collide(Particle& other) {
    sf::Vector2f impactVector = other.pos - pos;
    float d = magnitude(impactVector);
    if (d == 0.f) return; // avoid divide by zero, treat as no collision

    if (d < radius + other.radius) {
        float overlap = d - (radius + other.radius);
        sf::Vector2f dir = impactVector;
        dir = setMagnitude(dir, overlap / 2);
        pos += dir;
        other.pos -= dir;

        //d = radius + other.radius;

        float mSum = mass + other.mass;
        sf::Vector2f vDiff = other.vel - vel;

        float den = mSum * d * d;
        float num = vDiff.dot(impactVector);

        sf::Vector2f deltaVA = impactVector;
        deltaVA *= 2 * other.mass * num / den;
        vel += deltaVA;

        // Particle B (other)
        sf::Vector2f deltaVB = impactVector;
        deltaVB *= -2 * mass * num / den;
        other.vel += deltaVB;

		vel *= RESTITUTION;
		other.vel *= RESTITUTION;
    }

}


// batched rendering for faster rendering performance
void renderAll(sf::RenderWindow& window, const std::vector<Particle>& particles) {
    const int POINTS_PER_CIRCLE = 8;
    sf::VertexArray va(sf::PrimitiveType::Triangles, particles.size() * POINTS_PER_CIRCLE * 3);

    for (int i = 0; i < (int)particles.size(); i++) {
        const Particle& p = particles[i];
        sf::Vector2f center = p.pos + sf::Vector2f(p.radius, p.radius);

        for (int j = 0; j < POINTS_PER_CIRCLE; j++) {
            float a1 = (j / (float)POINTS_PER_CIRCLE) * 2 * PI;
            float a2 = ((j + 1) / (float)POINTS_PER_CIRCLE) * 2 * PI;
            // a2 - a1 is 45 degrees in radians, so we are making a triangle fan for each circle

            int idx = (i * POINTS_PER_CIRCLE + j) * 3;
            va[idx].position = center;
            va[idx + 1].position = center + sf::Vector2f(std::cos(a1), std::sin(a1)) * p.radius;
            va[idx + 2].position = center + sf::Vector2f(std::cos(a2), std::sin(a2)) * p.radius;

            va[idx].color = p.color;
            va[idx + 1].color = p.color;
            va[idx + 2].color = p.color;
        }
    }
    window.draw(va);
}

void spawnParticles(std::vector<Particle>& particles, std::vector<std::vector<int>>& grid, int amount) {
    int particlesPerRow = (int)(WIDTH / (PARTICLE_RADIUS * 3));
    for (int i = 0; i < amount; ++i) {
        int row = (int)(i / particlesPerRow);
        int col = i % particlesPerRow;
        particles.emplace_back(col * PARTICLE_RADIUS * 3 + PARTICLE_RADIUS, row * PARTICLE_RADIUS * 3 + PARTICLE_RADIUS, PARTICLE_RADIUS);
        grid[particles.back().key].push_back((int)particles.size() - 1);
    }
}