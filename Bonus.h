#pragma once

#include <SFML/Graphics.hpp>
#include <random>
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
    RandomTrajectory
};

class Bonus {
    ///генерирует случайные числа, но наверное стоило бы использовать просто rand
    static std::mt19937& getRNG();

public:
    Bonus(BonusType t, sf::Vector2f pos);
    void update(float dt);
    void draw(sf::RenderWindow& window);
    void apply(Platform& paddle, Ball& ball);

    sf::CircleShape shape;
    BonusType type;
    float speedY = 300.f;
};