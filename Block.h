#pragma once

#include <SFML/Graphics.hpp>
#include "Bonus.h"
#include <memory>

class Ball;

class Block {
public:
    Block(sf::Vector2f pos, sf::Vector2f size, sf::Color color);
    virtual ~Block() = default;

    virtual void draw(sf::RenderWindow& window);
    //virtual std::unique_ptr<Bonus> onHit();
    virtual std::unique_ptr<Bonus> onHit(Ball& ball);


    sf::Vector2f getCenter() const;
    bool isDestroyed() const;
    const sf::RectangleShape& getShape() const { return shape; }

protected:
    bool destroyed = false;
    sf::RectangleShape shape;
};


class IndestructibleBlock : public Block {
public:
    using Block::Block;
    // ***** MODIFIED SIGNATURE *****
    std::unique_ptr<Bonus> onHit(Ball& ball) override;
};

class HealthBlock : public Block {
public:
    HealthBlock(const sf::Vector2f& pos, const sf::Vector2f& size, const sf::Color& color, int hp);
    // ***** MODIFIED SIGNATURE *****
    std::unique_ptr<Bonus> onHit(Ball& ball) override;
protected:
    int health;
};

class SpeedBlock : public Block {
public:
    using Block::Block;
    // ***** MODIFIED SIGNATURE *****
    std::unique_ptr<Bonus> onHit(Ball& ball) override;
};

class BonusBlock : public HealthBlock {
public:
    BonusBlock(const sf::Vector2f& pos, const sf::Vector2f& size, const sf::Color& color, int hp, BonusType bt);
    // ***** MODIFIED SIGNATURE *****
    std::unique_ptr<Bonus> onHit(Ball& ball) override;
private:
    BonusType bonusType;
};