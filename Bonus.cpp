#include "Bonus.h"
#include "Utils.h"


std::mt19937& Bonus::getRNG() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    return gen;
}

Bonus::Bonus(BonusType t, sf::Vector2f pos): type(t) 
{

    shape.setRadius(10.f);
    shape.setOrigin(10.f, 10.f);
    shape.setPosition(pos);

    switch (type) 
    {
        case IncreasePaddle:     shape.setFillColor(sf::Color::Green);       break;
        case DecreasePaddle:     shape.setFillColor(sf::Color::Red);         break;
        case IncreaseBallSpeed:  shape.setFillColor(sf::Color::Yellow);      break;
        case DecreaseBallSpeed:  shape.setFillColor(sf::Color::Cyan);        break;
        case StickyPaddle:       shape.setFillColor(sf::Color::White);       break;
        case OneTimeBottom:      shape.setFillColor(sf::Color(128, 128, 0)); break;
        case RandomTrajectory:   shape.setFillColor(sf::Color::Blue);        break;
    }
}

void Bonus::update(float dt) {
    shape.move(0.f, speedY * dt);
}

void Bonus::draw(sf::RenderWindow& window) {
    window.draw(shape);
}

void Bonus::apply(Platform& paddle, Ball& ball) {
    switch (type) 
    {
    case IncreasePaddle:
        paddle.paddleWidth += 50.f;
        paddle.shape.setSize({ paddle.paddleWidth, paddle.shape.getSize().y });
         break;

    case DecreasePaddle:
        paddle.paddleWidth = std::max(50.f, paddle.paddleWidth - 50.f);
        paddle.shape.setSize({ paddle.paddleWidth, paddle.shape.getSize().y });
        break;

    case IncreaseBallSpeed:
        ball.velocity *= 1.2f;
        break;

    case DecreaseBallSpeed:
        ball.velocity *= 0.8f;
        break;

    case StickyPaddle:
        paddle.sticky = true;
        break;

    case OneTimeBottom:
        paddle.allowedBottomPass = 1;
        break;


    case RandomTrajectory:
        ball.randomTrajectoryActive = true;
        ball.randomTrajectoryTimer = 2.f; /// длится 2 сек
        
        ///интервал от 0.5 до 1.5 сек, через который мяч должен поменять направление
        ball.nextDirectionChangeTime = 0.5f + (getRNG()() % 1000) / 1000.f;
        break;
    default: break;
    }
    //paddle.allowedBottomPass = 0;
}