#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <ctime>
#include <algorithm>

#include "Platform.h"
#include "Ball.h"
#include "Block.h"
#include "Bonus.h"

class Game {
public:
    static Game& instance();
    Game();
    void run();
    sf::Clock deltaClock;

private:
    void handleEvents();
    void update(float dt);
    void render();
    void resetGame();      /// ����� ��������� ��� �����������
    void initText();

    sf::RenderWindow window;
    Platform paddle;
    Ball ball;
    std::vector<std::unique_ptr<Block>> blocks;
    std::vector<std::unique_ptr<Bonus>> bonuses;
    int score;
    int misses;

    /// ����� ���� ��� ������ � ������
    int lives;
    bool gameOver;
    sf::Font font;
    sf::Text livesText;
    sf::Text gameOverText;

};