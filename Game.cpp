#include "Game.h"
#include "Utils.h"
#include <iostream>
const int W = 700;
const int H = 900;
int speed = 600;

///по сути это просто main, но в отдельном файле, чтобы в самом мейне поменьше было 
Game::Game() : window(sf::VideoMode(W, H), "Arkanoid"),
    paddle((W - 200) / 2, 850, 200, 20, sf::Color::Red),
    ball(10), score(0), misses(0), lives(3), gameOver(false)
{
    window.setFramerateLimit(144);
       
    initText();     
    resetGame();
}

void Game::resetGame() {
    /// сброс всех игровых объектов
    blocks.clear();
    bonuses.clear();
    score = 0;
    misses = 0;
    lives = 3;
    gameOver = false;
    ball.stuck = true;
    paddle.sticky = false;

    /// создание блоков...
    const float gapX = 1.f; ///небольшой зазор между блоками
    const float gapY = 1.f;

    int cols = static_cast<int>((W + gapX) / (60 + gapX));
    int rows = 10;

    float blockWidth = (W - gapX * (cols - 1)) / cols;
    sf::Vector2f bSize(blockWidth, 20);

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            sf::Vector2f pos(j * (bSize.x + gapX), i * (bSize.y + gapY));

            bool indestructible = (i == 0 && j % 2 == 0);

///3ИЙ Пункт про ускоряющие блоки:
/// 
            /// 5% шанс, что это “ускоряющий” блок
            bool isSpeedBlock = (!indestructible && (getRNG()() % 20 == 0));
            bool hasBonus = (!indestructible && !isSpeedBlock && (getRNG()() % 3 == 0));
            BonusType bt = hasBonus
                ? static_cast<BonusType>(getRNG()() %6)
                : BonusType::RandomTrajectory; 

               sf::Color col = indestructible? sf::Color(100, 100, 100): isSpeedBlock? sf::Color(200, 0, 200)
                 : sf::Color(20, 50 + 20 * i, 30 + 20 * i);
            int hp = indestructible ? 999 : 1 + (i % 3);

            /// передаём флаг isSpeedBlock в hiddenBonus, чтобы не менять Block.h
           /// hiddenBonus==IncreaseBallSpeed мы будем трактовать как ускоряющий
           
            BonusType hidden = isSpeedBlock ? BonusType::IncreaseBallSpeed : bt;
            blocks.emplace_back(std::make_unique<Block>(pos, bSize, col, !indestructible, hp, hasBonus, hidden));
        }
    }
}

void Game::initText() {
    ///работа с текстом, и так все понятно
    if (!font.loadFromFile("arialmt.ttf")) {
        std::cerr << "Failed to load font arial.ttf" << std::endl;
    }
    livesText.setFont(font);
    livesText.setCharacterSize(30);
    livesText.setFillColor(sf::Color::White);
    livesText.setPosition(10.f, 10.f);
    livesText.setOutlineThickness(2.f);
    livesText.setOutlineColor(sf::Color::Black);

    gameOverText.setFont(font);
    gameOverText.setCharacterSize(48);
    gameOverText.setFillColor(sf::Color::Red);
    gameOverText.setString("GAME OVER\nClick to Restart");
    
    /// центрируем
    sf::FloatRect bounds = gameOverText.getLocalBounds();
    gameOverText.setOrigin(bounds.width / 2, bounds.height / 2);
    gameOverText.setPosition(W / 2.f, H / 2.f);
}

void Game::handleEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed ||
            sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
            window.close();
        }


        if (!gameOver &&
            event.type == sf::Event::KeyPressed &&
            event.key.code == sf::Keyboard::Space) {
            ball.launch();
        }
                         ///add reset game по ЛКМ
        if (gameOver && event.type == sf::Event::MouseButtonPressed &&
            event.mouseButton.button == sf::Mouse::Left) {
            resetGame();
        }
    }
}



void Game::update(float dt) {
    
    if (gameOver) return;

    paddle.move(dt);
    if (ball.stuck) {
        ball.shape.setPosition(paddle.getAttachPosition(ball.shape.getRadius()));
    }

    ball.update(dt);
    ball.handleWallCollisions();

    /// реализация столкновения с дном
    float bottom = ball.shape.getPosition().y + ball.shape.getRadius();
    if (bottom > H) {
        if (paddle.allowedBottomPass > 0) {
            /// отскок от дна при бонусе
            paddle.allowedBottomPass--;
            ball.velocity.y = -std::abs(ball.velocity.y);
        }
        else {
            /// минус жизнь
            lives--;
            if (lives <= 0) {
                gameOver = true;
                return;
            }
            else {
                ball.stuck = true;
            }
        }
    }

    if (ball.shape.getGlobalBounds().intersects(
        paddle.shape.getGlobalBounds())) {
        paddle.handleBallCollision(ball);
    }

    ///генерация блоков
    for (auto it = blocks.begin(); it != blocks.end();) {
        if (ball.shape.getGlobalBounds().intersects((*it)->shape.getGlobalBounds())) 
        {
            ball.velocity.y = -ball.velocity.y;
            if ((*it)->hiddenBonus == BonusType::IncreaseBallSpeed) {
                ball.velocity *= 1.05f;
          
            }
            score++;
            (*it)->hit();
            if ((*it)->isDestroyed()) 
            {
                if ((*it)->hasBonus) 
                {
                    bonuses.emplace_back(std::make_unique<Bonus>
                        (
                            (*it)->hiddenBonus,
                            (*it)->getCenter()
                        )
                    );
                }
                it = blocks.erase(it);
            }
            else 
            {
                ++it;
            }
        }

        else 
        {
            ++it;
        }
    }

    for (auto it = bonuses.begin(); it != bonuses.end();) {
        (*it)->update(dt);
        if ((*it)->shape.getGlobalBounds().intersects(paddle.shape.getGlobalBounds())) 
        {
            (*it)->apply(paddle, ball);
            it = bonuses.erase(it);
        }

        else if ((*it)->shape.getPosition().y > H) 
        {
            it = bonuses.erase(it);
        }
        else 
        {
            ++it;
        }
    }

    if (ball.shape.getPosition().y + ball.shape.getRadius() > H) {
        if (paddle.allowedBottomPass > 0) {
            paddle.allowedBottomPass--;
            ball.stuck = true;
        }
    }
}

void Game::render() {

    window.clear(sf::Color(20, 0, 100));
    paddle.draw(window);
    ball.draw(window);
    
    for (auto& b : blocks) b->draw(window);
    for (auto& bn : bonuses) bn->draw(window);
    
    ///рисую текст поверх блоков, поэтому такой порядок отрисовки
    livesText.setString("Lives: " + std::to_string(lives));
    window.draw(livesText);

    if (gameOver) {
        window.draw(gameOverText);
    }

    window.display();
}

void Game::run() {
    while (window.isOpen()) {
        handleEvents();
        float dt = deltaClock.restart().asSeconds();
        update(dt);
        render();
    }
}