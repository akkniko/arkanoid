#include "Platform.h"
#include "Ball.h"
#include <algorithm>
#include <SFML/Window/Keyboard.hpp>

extern int speed;

Platform::Platform(int x, int y, float w, float h, sf::Color col) : paddleWidth(w) 
{
    shape.setPosition(x, y);
    shape.setSize({ w, h });
    shape.setFillColor(col);
    shape.setOutlineColor(sf::Color::Black);
    shape.setOutlineThickness(3);
}

void Platform::draw(sf::RenderWindow& window) {
    window.draw(shape);
}

void Platform::move(float dt) {

    ///движение каретки по A и D
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        shape.move(-speed * dt, 0);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        shape.move(speed * dt, 0);

    float x = shape.getPosition().x;

    x = std::clamp(x, 0.f, 700.f - paddleWidth);

    shape.setPosition(x, shape.getPosition().y);
}


///угол отражения = углу падения
void Platform::handleBallCollision(Ball& ball) {
    ball.velocity.y = -std::abs(ball.velocity.y);
    if (sticky) {
        ball.stuck = true;
        sticky = false;
    }
}


sf::Vector2f Platform::getAttachPosition(float radius) const {
    return 
    { 
        shape.getPosition().x + shape.getSize().x / 2.f,   
        shape.getPosition().y - radius 
    };
}