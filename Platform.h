#pragma once

#include <SFML/Graphics.hpp>
class Ball;

class Platform {
public:
    Platform(int x, int y, float w, float h, sf::Color col);
    void draw(sf::RenderWindow& window);
    void move(float dt);
    void handleBallCollision(Ball& ball);
    sf::Vector2f getAttachPosition(float radius) const;

    sf::RectangleShape shape;
    sf::RectangleShape getShape() { return shape; };
    float paddleWidth;
    bool sticky = false;
    int allowedBottomPass = 0;
};