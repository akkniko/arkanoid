#include "Block.h"


Block::Block(sf::Vector2f pos, sf::Vector2f size, sf::Color col) {
    shape.setPosition(pos);
    shape.setSize(size);
    shape.setFillColor(col);
}

void Block::draw(sf::RenderWindow& window) {
    window.draw(shape);
}

sf::Vector2f Block::getCenter() const {
    return shape.getPosition() + shape.getSize() / 2.f;
}

bool Block::isDestroyed() const {
    return destroyed;
}

std::unique_ptr<Bonus> Block::onHit(Ball& ball) {
    destroyed = true;
    return nullptr;
}



std::unique_ptr<Bonus> IndestructibleBlock::onHit(Ball& ball) {
    return nullptr;
}

HealthBlock::HealthBlock(const sf::Vector2f& pos, const sf::Vector2f& size, const sf::Color& color, int hp)
    : Block(pos, size, color), health(hp) {
}

std::unique_ptr<Bonus> HealthBlock::onHit(Ball& ball) {
    if (--health <= 0) {
        destroyed = true;
    }
    return nullptr;
}

std::unique_ptr<Bonus> SpeedBlock::onHit(Ball& ball) {
    destroyed = true;
    ball.velocity.x *= 1.1f;
    ball.velocity.y *= 1.1f;
    return nullptr;
}

BonusBlock::BonusBlock(const sf::Vector2f& pos, const sf::Vector2f& size, const sf::Color& color, int hp, BonusType type)
    : HealthBlock(pos, size, color, hp), bonusType(type) {
}

std::unique_ptr<Bonus> BonusBlock::onHit(Ball& ball) {
    HealthBlock::onHit(ball); 

    if (isDestroyed()) {
        switch (bonusType) {
        case BonusType::IncreasePaddle:
            return std::make_unique<IncreasePaddleBonus>(getCenter());
        case BonusType::DecreasePaddle:
            return std::make_unique<DecreasePaddleBonus>(getCenter());
        case BonusType::IncreaseBallSpeed:
            return std::make_unique<IncreaseBallSpeedBonus>(getCenter());
        case BonusType::DecreaseBallSpeed:
            return std::make_unique<DecreaseBallSpeedBonus>(getCenter());
        case BonusType::StickyPaddle:
            return std::make_unique<StickyPaddleBonus>(getCenter());
        case BonusType::OneTimeBottom:
            return std::make_unique<OneTimeBottomBonus>(getCenter());
        case BonusType::RandomTrajectory:
            return std::make_unique<RandomTrajectoryBonus>(getCenter());
        default:
            return nullptr;
        }
    }
    return nullptr;
}