#pragma once

#include <SFML/Graphics.hpp>
#include <random>
#include "Platform.h"
#include "Ball.h"


///5 � 6, 7 ������
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
    ///���������� ��������� �����, �� �������� ������ �� ������������ ������ rand
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