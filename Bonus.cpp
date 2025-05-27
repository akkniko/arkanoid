#include "Bonus.h"
#include "Utils.h"


Bonus::Bonus(sf::Vector2f pos) {
    shape.setRadius(10.f);
    shape.setOrigin(10.f, 10.f);
    shape.setPosition(pos);
}

void Bonus::update(float dt) {
    shape.move(0.f, speedY * dt);
}

void Bonus::draw(sf::RenderWindow& window) {
    window.draw(shape);
}

IncreasePaddleBonus::IncreasePaddleBonus(sf::Vector2f pos) : Bonus(pos) {
    shape.setFillColor(sf::Color::Green);
}
void IncreasePaddleBonus::apply(Platform& paddle, Ball& ball) {
    paddle.paddleWidth += 50.f;
    paddle.shape.setSize({ paddle.paddleWidth, paddle.shape.getSize().y });
}

DecreasePaddleBonus::DecreasePaddleBonus(sf::Vector2f pos) : Bonus(pos) {
    shape.setFillColor(sf::Color::Red);
}
void DecreasePaddleBonus::apply(Platform& paddle, Ball& ball) {
    paddle.paddleWidth = std::max(50.f, paddle.paddleWidth - 50.f);
    paddle.shape.setSize({ paddle.paddleWidth, paddle.shape.getSize().y });
}

IncreaseBallSpeedBonus::IncreaseBallSpeedBonus(sf::Vector2f pos) : Bonus(pos) {
    shape.setFillColor(sf::Color::Yellow);
}
void IncreaseBallSpeedBonus::apply(Platform& paddle, Ball& ball) {
    ball.velocity *= 1.2f;
}

DecreaseBallSpeedBonus::DecreaseBallSpeedBonus(sf::Vector2f pos) : Bonus(pos) {
    shape.setFillColor(sf::Color::Cyan);
}
void DecreaseBallSpeedBonus::apply(Platform& paddle, Ball& ball) {
    ball.velocity *= 0.8f;
}

StickyPaddleBonus::StickyPaddleBonus(sf::Vector2f pos) : Bonus(pos) {
    shape.setFillColor(sf::Color::White);
}
void StickyPaddleBonus::apply(Platform& paddle, Ball& ball) {
    paddle.sticky = true;
}

OneTimeBottomBonus::OneTimeBottomBonus(sf::Vector2f pos) : Bonus(pos) {
    shape.setFillColor(sf::Color(128, 128, 0)); // Olive
}
void OneTimeBottomBonus::apply(Platform& paddle, Ball& ball) {
    paddle.allowedBottomPass = 1;
}

RandomTrajectoryBonus::RandomTrajectoryBonus(sf::Vector2f pos) : Bonus(pos) {
    shape.setFillColor(sf::Color::Blue);
}
void RandomTrajectoryBonus::apply(Platform& paddle, Ball& ball) {
    ball.randomTrajectoryActive = true;
    ball.randomTrajectoryTimer = 2.f; // Lasts 2 seconds
    ball.nextDirectionChangeTime = 0.5f + (getRNG()() % 1000) / 1000.f;
}
