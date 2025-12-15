#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <memory>   // For std::unique_ptr
#include <random>   // For spawning enemies
#include <optional> // For the SFML 3 event loop

#include "config.h"
#include "Player.h"
#include "Enemy.h"
#include "Bullet.h"
#include "Laser.h"
#include "Shooter.h"

// 点积计算
float dot(const sf::Vector2f& a, const sf::Vector2f& b) {
    return a.x * b.x + a.y * b.y;
}

// 分离轴定理（SAT）碰撞检测
bool satIntersect(const std::vector<sf::Vector2f>& poly1, const std::vector<sf::Vector2f>& poly2) {
    // 检查多边形1的每条边
    for (size_t i = 0; i < poly1.size(); i++) {
        size_t next = (i + 1) % poly1.size();
        sf::Vector2f edge = poly1[next] - poly1[i];
        sf::Vector2f normal(-edge.y, edge.x); // 法向量
        
        // 投影多边形1到法向量上
        float min1 = std::numeric_limits<float>::max();
        float max1 = std::numeric_limits<float>::lowest();
        for (const auto& point : poly1) {
            float projection = dot(normal, point);
            min1 = std::min(min1, projection);
            max1 = std::max(max1, projection);
        }
        
        // 投影多边形2到法向量上
        float min2 = std::numeric_limits<float>::max();
        float max2 = std::numeric_limits<float>::lowest();
        for (const auto& point : poly2) {
            float projection = dot(normal, point);
            min2 = std::min(min2, projection);
            max2 = std::max(max2, projection);
        }
        
        // 检查投影是否重叠
        if (max1 < min2 || max2 < min1) {
            return false; // 发现分离轴，没有碰撞
        }
    }
    // 检查多边形2的每条边
    for (size_t i = 0; i < poly2.size(); i++) {
        size_t next = (i + 1) % poly2.size();
        sf::Vector2f edge = poly2[next] - poly2[i];
        sf::Vector2f normal(-edge.y, edge.x);
        
        float min1 = std::numeric_limits<float>::max();
        float max1 = std::numeric_limits<float>::lowest();
        for (const auto& point : poly1) {
            float projection = dot(normal, point);
            min1 = std::min(min1, projection);
            max1 = std::max(max1, projection);
        }
        
        float min2 = std::numeric_limits<float>::max();
        float max2 = std::numeric_limits<float>::lowest();
        for (const auto& point : poly2) {
            float projection = dot(normal, point);
            min2 = std::min(min2, projection);
            max2 = std::max(max2, projection);
        }
        
        if (max1 < min2 || max2 < min1) {
            return false;
        }
    }
    
    return true; // 所有轴都重叠，发生碰撞
}

int main() {    
    // Create the window
    sf::RenderWindow window(sf::VideoMode({W_WIDTH, W_HEIGHT}), "Fly Plane Game");
    window.setFramerateLimit(W_FRAME_RATE_LIMIT);
    auto player = std::make_unique<Player>(W_WIDTH / 2 - P_SIZE / 2, W_HEIGHT - P_SIZE - W_BORDER, P_SPEED);
    // Clocks for timing. 'dt' (delta time) and spawn logic.
    sf::Clock clock;
    sf::Clock spawnClock; // Timer to spawn new enemies
    bool gameOver = false;
    int score = 0;
    std::vector<std::unique_ptr<Bullet>> playerBullets;
    std::vector<std::unique_ptr<Enemy>> enemies;
    std::vector<std::unique_ptr<Bullet>> enemyBullets;
    std::vector<std::unique_ptr<Laser>> lasers;
    sf::Clock enemySpawnClock;
    sf::Clock LaserSpawnClock;
    std::default_random_engine randomEngine;
    randomEngine.seed(static_cast<unsigned int>(std::time(nullptr)));
    // --- Game Loop ---
    while (window.isOpen()) {
    // 计算上一帧到现在的时间
    sf::Time dt = clock.restart();
    if (gameOver) dt = sf::Time::Zero; // 游戏结束时停止更新
    // --- 1. 事件处理 ---
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }
        
        if (!gameOver) {
            player->handleEvent(event);
        }
        if (gameOver && event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::R) {
                // 重置游戏状态
                gameOver = false;
                score = 0;
            
                // 重新创建玩家
                player = std::make_unique<Player>(W_WIDTH / 2 - P_SIZE / 2, W_HEIGHT - P_SIZE - W_BORDER, P_SPEED);
            
                // 清空所有游戏对象
                playerBullets.clear();
                enemies.clear();
                enemyBullets.clear();
            
                // 重置计时器
                clock.restart();
                enemySpawnClock.restart();
            
                // 重置随机数生成器
                randomEngine.seed(static_cast<unsigned int>(std::time(nullptr)));
            }
        }
    }
    
    // --- 2. 游戏逻辑更新 ---
    if (!gameOver) {
        // 更新玩家（包括鼠标跟随）
        player->update(dt.asSeconds());
        
        // 自动连续发射子弹（放在这里，每帧检查）
        if (auto newBullet = player->shoot()) {
            playerBullets.push_back(std::move(newBullet));
        }
        float currentTime = enemySpawnClock.getElapsedTime().asSeconds();
        if (currentTime >= E_SPAWN_TIME) {
            enemySpawnClock.restart();
                
            // 生成随机位置
            std::uniform_real_distribution<float> distX(E_RADIUS, W_WIDTH - E_RADIUS);
            float spawnX = distX(randomEngine);
            float spawnY = -E_RADIUS * 2;
                
            // 创建新敌机
            std::uniform_real_distribution<float> speed(0.0f, 40.0f);
            auto newEnemy = std::make_unique<Enemy>(spawnX, spawnY, E_SPEED);
            newEnemy->addspeed(speed(randomEngine));
            enemies.push_back(std::move(newEnemy));
        }
        float laserTime = LaserSpawnClock.getElapsedTime().asSeconds();
        if (laserTime >= L_FIRE_COOLDOWN) {
            LaserSpawnClock.restart();
            // 创建激光对象
            sf::FloatRect playerBounds = player->getBounds();
            float playerCenterX = playerBounds.left + playerBounds.width / 2;
            float playerCenterY = playerBounds.top + playerBounds.height / 2;
            auto laser = std::make_unique<Laser>(playerCenterX, playerCenterY, 0.0f);
            lasers.push_back(std::move(laser));
        }
        // 更新所有敌机
        for(auto& enemy : enemies) {
            enemy->update(dt.asSeconds());
        }
        // 敌机射击
        for(auto& enemy : enemies) {
            if (auto newBullet = enemy->shoot(0)) {
                enemyBullets.push_back(std::move(newBullet));
                enemy->addcooldown(-2.0f);
                if (auto newBullet = enemy->shoot(-1)) {
                    enemyBullets.push_back(std::move(newBullet));
                }
                enemy->addcooldown(-2.0f);
                if (auto newBullet = enemy->shoot(1)) {
                    enemyBullets.push_back(std::move(newBullet));
                }
            }
            
        }
        // 更新所有子弹
        for (auto& bullet : playerBullets) {
            bullet->update(dt.asSeconds());
        }
        // 更新所有激光
        for (auto& laser : lasers) {
            laser->update(dt.asSeconds());
        }
        //判断玩家子弹与敌机的碰撞
        for (auto& bullet : playerBullets) {
            for (auto& enemy : enemies) {
                if (bullet->getBounds().intersects(enemy->getBounds())) {
                    enemy->takeDamage(B_DAMAGE);
                    bullet->disappear(); // 标记子弹为无效
                }
            }
        }
        for(auto& bullet : enemyBullets) {
            bullet->update(dt.asSeconds());
        }
        //判断敌机子弹与玩家的碰撞
        for (auto& bullet : enemyBullets) {
            if (bullet->getBounds().intersects(player->getBounds())) {
                gameOver = true; // 玩家被击中，游戏结束
            }
        }
        //判断敌机与玩家的碰撞
        for (auto& enemy : enemies) {
            if (enemy->getBounds().intersects(player->getBounds())) {
                gameOver = true; // 玩家与敌机碰撞，游戏结束
            }
        }
        //判断激光与玩家的碰撞
        for (auto& laser : lasers) {
            if (laser->isAlive() && laser->isActive()) {
                // 获取激光旋转后的矩形顶点
                std::vector<sf::Vector2f> laserPoints = laser->getTransformedPoints();
        
                // 获取玩家三角形的顶点
                std::vector<sf::Vector2f> playerPoints = player->getPreciseBounds();
        
                // 使用SAT进行精确碰撞检测
                if (satIntersect(laserPoints, playerPoints)) {
                    gameOver = true;
                    break; // 一旦检测到碰撞就退出循环
                }
            }
        }
        // 移除已失效的子弹
        playerBullets.erase(
            std::remove_if(playerBullets.begin(), playerBullets.end(),
                [](const std::unique_ptr<Bullet>& bullet) {
                    return !bullet->isAlive();
                }),
            playerBullets.end()
        );
        // 移除已失效的激光
        lasers.erase(
            std::remove_if(lasers.begin(), lasers.end(),
                [](const std::unique_ptr<Laser>& laser) {
                    return !laser->isAlive();
                }),
            lasers.end()
        );
        // 移除已失效的敌机
        enemies.erase(
            std::remove_if(enemies.begin(), enemies.end(),
                [](const std::unique_ptr<Enemy>& enemy) {
                    return !enemy->isAlive();
                }),
            enemies.end()
        );
    }

    // --- 3. 渲染 ---
    window.clear(sf::Color::Black); 
    // 绘制玩家
    player->draw(window);
    // 绘制所有子弹
    for (auto& bullet : playerBullets) {
        bullet->draw(window);
    }
    for (auto& bullet : enemyBullets) {
        bullet->draw(window);
    }
    for (auto& enemy : enemies) {
        enemy->draw(window);
    }
    for (auto& laser : lasers) {
        laser->draw(window);
    }
    window.display();
    }
}