#include "Block.h"

Block::Block(sf::Vector2f pos, sf::Vector2f size, sf::Color col,
    bool destr, int hp, bool bonus, BonusType bt) : destructible(destr), health(hp), hiddenBonus(bt), hasBonus(bonus) 
{
    shape.setPosition(pos);
    shape.setSize(size);
    shape.setFillColor(col);
    shape.setOutlineColor(sf::Color::Black);
    shape.setOutlineThickness(2);
}

void Block::draw(sf::RenderWindow& window) 
{
    window.draw(shape);
}

void Block::hit() {
    if (destructible) health--;
}

bool Block::isDestroyed() const {
    return destructible && health <= 0;
}

sf::Vector2f Block::getCenter() const {
    return shape.getPosition() + shape.getSize() / 2.f;
}