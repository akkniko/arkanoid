#pragma once

#include <SFML/Graphics.hpp>
#include "Bonus.h"

class Block {
public:

    Block(sf::Vector2f pos, sf::Vector2f size, sf::Color col, bool destr, 
        int hp = 1, bool bonus = false, BonusType bt = IncreasePaddle);

    void draw(sf::RenderWindow& window);
    void hit();
    bool isDestroyed() const;
    sf::Vector2f getCenter() const;

    sf::RectangleShape shape;
    
    ///1�� �����
    bool destructible;

    ///4�� �����
    int health;

    BonusType hiddenBonus;

    ///2�� �����
    bool hasBonus;
};