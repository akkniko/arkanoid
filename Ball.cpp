#include "Ball.h"
#include <random>
#include "Utils.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

//static std::mt19937& getRNG() {
//    static std::random_device rd;
//    static std::mt19937 gen(rd());
//    return gen;
//} TODO: сделать это отдельным файлом, а не здесь   



Ball::Ball(float r) {
    shape.setRadius(r);
    shape.setOrigin(r, r);
    shape.setFillColor(sf::Color::White);
    shape.setOutlineColor(sf::Color::Red);
    shape.setOutlineThickness(1);
}

void Ball::launch() {
    if (stuck) {

        ///random dir Ox on launch ball
        velocity = { 400.f * ((getRNG()() % 2) == 0 ? -1 : 1), -400.f };
        stuck = false;
    }
}

void Ball::update(float dt) {
    ///реализаци€ случайного направлени€ движени€ м€чика, 7пункт из тз
    if (randomTrajectoryActive) {
        randomTrajectoryTimer -= dt;
        nextDirectionChangeTime -= dt;
        if (nextDirectionChangeTime <= 0.f) {
            double currentSpeed = std::sqrt(velocity.x * velocity.x + velocity.y * velocity.y);

            ///   новый случайный угол  от 100 до 320 градусов
            double angle = (getRNG()() % 180) * (M_PI / 180.f);
            velocity.x = currentSpeed * std::cos(angle);
            velocity.y = -currentSpeed * std::sin(angle);


            /// сброс таймера 
            nextDirectionChangeTime = 1.f + (getRNG()() % 1000) / 1000.f;

           /* float dirX = ((getRNG()() % 2) == 0 ? -1.f : 1.f);
            float dirY = ((getRNG()() % 2) == 0 ? -1.f : 1.f);
            velocity = { 500.f * dirX,  -500.f * dirY };
            velocity = -velocity;
            nextDirectionChangeTime = 1.f + (getRNG()() % 1000) / 1000.f;*/

        }
        if (randomTrajectoryTimer <= 0.f) {
            randomTrajectoryActive = false;
        }
    }
    if (!stuck) shape.move(velocity * dt);
}

void Ball::draw(sf::RenderWindow& window) {
    window.draw(shape);
}

void Ball::handleWallCollisions() {
    auto pos = shape.getPosition();
    float r = shape.getRadius();

    ///размеры экрана у мен€ в Game.cpp, но если подключить - крашнетс€ программа
    ///поэтому сделал ручной ввод, что, конечно, не очень правильно
    if (pos.x - r < 0 || pos.x + r > 700) velocity.x = -velocity.x;
    if (pos.y - r < 0) velocity.y = -velocity.y;
}