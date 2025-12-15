#pragma once

#include "GameObject.h"
#include "Shooter.h"  // 包含发射器类
#include "Bullet.h"
#include <SFML/Graphics.hpp>

class Player : public GameObject {
public:    
    Player(float x, float y, float speed);

    void handleEvent(const sf::Event& event) override;
    void update(float dt) override;
    void draw(sf::RenderWindow& window) const override;
    sf::FloatRect getBounds() const override;
    
    // 发射子弹函数
    std::unique_ptr<Bullet> shoot();
    std::vector<sf::Vector2f> getPreciseBounds() const;
    
    // 设置窗口边界（用于限制移动范围）
    void setWindowBounds(const sf::Vector2u& windowSize);

private:
    sf::CircleShape shape_;
    float speed_;
    Shooter shooter_;  // 发射器实例
    // 窗口边界限制
    sf::FloatRect windowBounds_;
    // 鼠标控制相关
    bool useMouseControl_ = true;  // 默认为鼠标控制
    sf::Vector2f targetPosition_; // 目标位置（鼠标位置）
    float smoothingFactor_ = 8.0f; // 平滑因子
};