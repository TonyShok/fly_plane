#include <SFML/Graphics.hpp>
#include "GameObject.h"
#include "Laser.h"
#include "config.h"
#include <random>

Laser::Laser(float x, float y, float angle) {
    width = L_INITIAL_WIDTH;
    shape_.setSize({width, L_HEIGHT});
    shape_.setFillColor(sf::Color::Blue);
    shape_.setOrigin(width / 2, L_HEIGHT / 2);
    shape_.setPosition(x, y);
    std::random_device rd;  // 硬件随机数生成器
    std::mt19937 gen(rd()); // 梅森旋转算法生成器
    std::uniform_int_distribution<> dis(-45, 45); // 均匀分布  
    shape_.setRotation(dis(gen));
}
std::vector<sf::Vector2f> Laser::getTransformedPoints() const {
    std::vector<sf::Vector2f> points;
    sf::FloatRect localBounds = shape_.getLocalBounds();
    
    // 获取矩形的四个角点（局部坐标）
    sf::Vector2f topLeft(0, 0);
    sf::Vector2f topRight(localBounds.width, 0);
    sf::Vector2f bottomRight(localBounds.width, localBounds.height);
    sf::Vector2f bottomLeft(0, localBounds.height);
    
    // 应用变换（位置、旋转、原点偏移）
    sf::Transform transform = shape_.getTransform();
    
    points.push_back(transform.transformPoint(topLeft));
    points.push_back(transform.transformPoint(topRight));
    points.push_back(transform.transformPoint(bottomRight));
    points.push_back(transform.transformPoint(bottomLeft));
    
    return points;
}

void Laser::update(float dt) {
    // 激光宽度增长逻辑
    if (grow_) {
        width += L_GROWTH_RATE * dt;
        if (width >= L_MAX_WIDTH) {
            width = L_MAX_WIDTH;
            grow_ = false;
            active_ = true;
            alive_ = true; // 激光达到最大宽度后消失
        }
    } else {
        active_ = false;
        width -= L_GROWTH_RATE * dt;
        if (width <= 0) {
            width = L_INITIAL_WIDTH;
            grow_ = true;
            alive_ = false; // 激光宽度减小到0后消失
        }
    }
    shape_.setSize({width, L_HEIGHT});
    shape_.setOrigin(width / 2, L_HEIGHT / 2);
}

void Laser::draw(sf::RenderWindow& window) const {
    if (alive_) {
        window.draw(shape_);
    }
}
sf::FloatRect Laser::getBounds() const {
    return shape_.getGlobalBounds();
}
