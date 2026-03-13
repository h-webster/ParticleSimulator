#include <SFML/Graphics.hpp>
#include <vector>
#include <random>
#include <cmath>
#include <iostream>
#include "particle.h"
#include "utils.h"
#include "ui.h"
#include "uiManager.h"

int main()
{

    const int FPS = 240;

    sf::RenderWindow window(sf::VideoMode({ WIDTH, HEIGHT }), "Particle Simulation!");
    window.setFramerateLimit(FPS);

    sf::Vector2f lastMousePos;
	sf::Vector2f mouseVel;

    std::vector<Particle> particles;

    std::vector<std::vector<int>> grid(cols * rows);
    sf::Clock clock;

    sf::Font font;
    if (!font.openFromFile("C:/Windows/Fonts/arial.ttf")) {
        std::cout << "Failed to load font";
    }


    uiManager ui;
    ui.initUI(font);

	spawnParticles(particles, grid, 3000);


    std::cout << "Started";
    while (window.isOpen())
    {
        float dt = clock.restart().asSeconds();
        dt = std::min(dt, 0.02f); // cap delta time to avoid big jumps

        float fps = (dt > 0) ? 1.0f / dt : 0.0f;
        std::string fpsString = "FPS: " + std::to_string(static_cast<int>(fps)) +
			" | Particles: " + std::to_string(particles.size());
        ui.update(fpsString, window);
       
        while (const auto e = window.pollEvent()) {
            if (e->is<sf::Event::Closed>())
                window.close();

            if (const auto* mousePress = e->getIf<sf::Event::MouseButtonPressed>()) {
                if (mousePress->button == sf::Mouse::Button::Left) {
                    float x = static_cast<float>(mousePress->position.x);
                    float y = static_cast<float>(mousePress->position.y);

                    // make it so you can't spawn particles on top of each other
                    bool canSpawn = true;
                    for (auto& p : particles) {
                        if (distance(p.pos + sf::Vector2f(p.radius, p.radius), { x, y }) < p.radius * 2) {
                            canSpawn = false;
                            break;
                        }
                    }
                    if (canSpawn) {
                        particles.emplace_back(x, y, PARTICLE_RADIUS);
                    }

                }
            }

            if (const auto* keyPress = e->getIf<sf::Event::KeyPressed>()) {
                if (keyPress->code == sf::Keyboard::Key::Space) {
                    particles.clear();
                }
				else if (keyPress->code == sf::Keyboard::Key::C) {
                    spawnParticles(particles, grid, 100);
                }
            }

			ui.handleEvent(*e, window);
        }

        window.clear(sf::Color(20, 20, 20));

        // mouse repel 
        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        
        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right)) {
            for (auto& p : particles) {
				sf::Vector2f dir = (p.pos + sf::Vector2f(p.radius, p.radius)) - mousePos;
				float d = magnitude(dir);
				if (d > 0.f && d < 300.f) {
                    dir = setMagnitude(dir, 500.f / (d)); // stronger repulsion when closer
					if (magnitude(dir) + magnitude(p.vel) > 400.f) { // max velocity cap to prevent extreme speeds
                        continue;
                    }
                    p.vel += dir;

                }
            }
        }

        // update physics first, then render, then update grid for collisions

        for (auto& p : particles) {
            p.update(dt);
        }
        renderAll(window, particles);

        // clear grid
        for (auto& cell : grid) { cell.clear(); }

        for (int i = 0; i < particles.size(); ++i) {
            particles[i].updateCell();;
            int key = particles[i].key;
            if (key >= 0 && key < grid.size()) {
                grid[key].push_back(i);
            }
        }

        // check collisions using hash grid
        for (int i = 0; i < particles.size(); i++) {
            Particle& p = particles[i];
            int cx = p.cell.x;
            int cy = p.cell.y;

            for (int dx = -1; dx <= 1; dx++) {
                for (int dy = -1; dy <= 1; dy++) {
                    int nx = cx + dx;
                    int ny = cy + dy;

                    if (nx < 0 || nx >= cols || ny < 0 || ny >= rows) continue;

                    int neighborKey = nx + ny * cols;
                    for (int j : grid[neighborKey]) {
                        if (j <= i) continue;
                        p.collide(particles[j]);
                    }
                }
            }
        }

        ui.render(window);
        window.display();
    }
}