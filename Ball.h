#pragma once

#include <SFML/Graphics.hpp>


class Ball {
public:
    Ball(float r = 10);
    void launch();
    void update(float dt);
    void draw(sf::RenderWindow& window);
    void handleWallCollisions();

    sf::CircleShape shape;
    sf::Vector2f velocity;
    bool stuck = true;

    sf::CircleShape getShape() { return shape; };

    /// Для случайной траектории, 7 бонус
    bool randomTrajectoryActive = false;
    float randomTrajectoryTimer = 0.f;
    float nextDirectionChangeTime = 0.f;
};