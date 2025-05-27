#include "Game.h"
#include "Utils.h" 
#include <iostream>

const int W = 700;
const int H = 900;
int speed = 600; 

Game& Game::instance() {
    static Game instance;
    return instance;
}

Game::Game() : window(sf::VideoMode(W, H), "Arkanoid"),
paddle((W - 200) / 2, 850, 200, 20, sf::Color::Red),
ball(10), score(0), misses(0), lives(3), gameOver(false)
{
    window.setFramerateLimit(144);
    initText();
    resetGame();
}

void Game::resetGame() {
    blocks.clear();
    bonuses.clear();

    score = 0;
    lives = 3; 
    gameOver = false;
    ball.stuck = true;
    paddle.paddleWidth = 200; 
    paddle.shape.setSize({ paddle.paddleWidth, paddle.shape.getSize().y });
    paddle.sticky = false;
    paddle.allowedBottomPass = 0;
   
    const float gapX = 1.f, gapY = 1.f;
    int cols = int((W + gapX) / (60 + gapX));
    int rows = 10;
    float blockWidth = (W - gapX * (cols - 1)) / cols;
    sf::Vector2f bSize(blockWidth, 20.f);

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            sf::Vector2f pos(j * (bSize.x + gapX), i * (bSize.y + gapY));

            bool isIndestructible = (i == 0 && j % 2 == 0);
            bool isSpeedBlock = (!isIndestructible && getRNG()() % 12 == 0); 
            bool isBonusBlock = (!isIndestructible && !isSpeedBlock && getRNG()() % 3 == 0); 

            sf::Color col = sf::Color(20, 50 + 20 * i, 30 + 20 * i); 
            int hp = 1 + (i % 3);

            std::unique_ptr<Block> newBlock;

            if (isIndestructible) {
                col = sf::Color(100, 100, 100);
                newBlock = std::make_unique<IndestructibleBlock>(pos, bSize, col);
            }
            else if (isSpeedBlock) {
                col = sf::Color(200, 0, 200); 
                newBlock = std::make_unique<SpeedBlock>(pos, bSize, col);
            }
            else if (isBonusBlock) {
                BonusType randomBonus = static_cast<BonusType>(getRNG()() % static_cast<int>(BonusType::Count));
                newBlock = std::make_unique<BonusBlock>(pos, bSize, col, hp, randomBonus);
            }
            else {
                newBlock = std::make_unique<HealthBlock>(pos, bSize, col, hp);
            }
            blocks.push_back(std::move(newBlock));
        }
    }
    if (ball.stuck) {
        ball.shape.setPosition(paddle.getAttachPosition(ball.shape.getRadius()));
    }
}



void Game::initText() {
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
            (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)) {
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

    float ballBottom = ball.shape.getPosition().y + ball.shape.getRadius();
    if (ballBottom > H) { 
        if (paddle.allowedBottomPass > 0) {
            paddle.allowedBottomPass--;
            ball.velocity.y = -std::abs(ball.velocity.y);
        }
        else {
            lives--;
            if (lives <= 0) {
                gameOver = true;
                return;
            }
            ball.stuck = true; 
            ball.shape.setPosition(paddle.getAttachPosition(ball.shape.getRadius()));
        }
    }

    if (ball.shape.getGlobalBounds().intersects(paddle.shape.getGlobalBounds())) {
        paddle.handleBallCollision(ball); 
    }

    for (auto it = blocks.begin(); it != blocks.end();) {
        Block* b = it->get();
        if (ball.shape.getGlobalBounds().intersects(b->getShape().getGlobalBounds())) {
            ball.velocity.y = -ball.velocity.y;
            score++;

            auto bonusPtr = b->onHit(ball);
            if (bonusPtr) {
                bonuses.push_back(std::move(bonusPtr));
            }

            if (b->isDestroyed()) {
                it = blocks.erase(it);
            }
            else {
                ++it;
            }
        }
        else {
            ++it;
        }
    }

    for (auto it = bonuses.begin(); it != bonuses.end();) {
        Bonus* bn = it->get();
        bn->update(dt);

        if (bn->getShape().getGlobalBounds().intersects(paddle.shape.getGlobalBounds())) {
            bn->apply(paddle, ball);
            it = bonuses.erase(it);
        }
        else if (bn->getShape().getPosition().y > H) { 
            it = bonuses.erase(it);
        }
        else {
            ++it;
        }
    }
    if (ball.stuck) {
        ball.shape.setPosition(paddle.getAttachPosition(ball.shape.getRadius()));
    }
}

void Game::render() {

    window.clear(sf::Color(20, 0, 100));

    paddle.draw(window);
    ball.draw(window);

    for (auto& b : blocks) {
        b->draw(window);
    }
    for (auto& bn : bonuses) {
        bn->draw(window);
    }

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