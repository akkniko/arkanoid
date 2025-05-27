#pragma once

#include <SFML/Graphics.hpp>
#include <random>
#include <memory>
#include "Platform.h"
#include "Ball.h"


///5 и 6, 7 пункты
enum BonusType {
    IncreasePaddle,
    DecreasePaddle,
    IncreaseBallSpeed,
    DecreaseBallSpeed,
    StickyPaddle,
    OneTimeBottom,
    RandomTrajectory,
    Count
};

class Bonus {
public:
    Bonus(sf::Vector2f pos);
    virtual ~Bonus() = default;

    void update(float dt);
    void draw(sf::RenderWindow& window);
    const sf::CircleShape& getShape() const { return shape; }

    virtual void apply(Platform& paddle, Ball& ball) = 0;

protected:
    sf::CircleShape shape;
    float speedY = 300.f;
};


class IncreasePaddleBonus : public Bonus {
public:
    IncreasePaddleBonus(sf::Vector2f pos);
    void apply(Platform& paddle, Ball& ball) override;
};

class DecreasePaddleBonus : public Bonus {
public:
    DecreasePaddleBonus(sf::Vector2f pos);
    void apply(Platform& paddle, Ball& ball) override;
};

class IncreaseBallSpeedBonus : public Bonus {
public:
    IncreaseBallSpeedBonus(sf::Vector2f pos);
    void apply(Platform& paddle, Ball& ball) override;
};

class DecreaseBallSpeedBonus : public Bonus {
public:
    DecreaseBallSpeedBonus(sf::Vector2f pos);
    void apply(Platform& paddle, Ball& ball) override;
};

class StickyPaddleBonus : public Bonus {
public:
    StickyPaddleBonus(sf::Vector2f pos);
    void apply(Platform& paddle, Ball& ball) override;
};

class OneTimeBottomBonus : public Bonus {
public:
    OneTimeBottomBonus(sf::Vector2f pos);
    void apply(Platform& paddle, Ball& ball) override;
};

class RandomTrajectoryBonus : public Bonus {
public:
    RandomTrajectoryBonus(sf::Vector2f pos);
    void apply(Platform& paddle, Ball& ball) override;
};