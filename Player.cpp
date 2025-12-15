#include "Player.h"
#include "config.h"
#include "Shooter.h"
#include "Bullet.h"

Player::Player(float x, float y, float speed) 
    : speed_(speed), 
      shooter_(sf::seconds(P_FIRE_COOLDOWN),0)  // 使用配置的发射间隔
{
    shape_.setPointCount(3);
    shape_.setRadius(P_SIZE / 2.0f);
    shape_.setFillColor(sf::Color::Green);
    shape_.setPosition(x + P_SIZE/2, y + P_SIZE/2);
    
    // 初始窗口边界（会在setWindowBounds中更新）
    windowBounds_ = sf::FloatRect(0, 0, W_WIDTH, W_HEIGHT);
}

std::vector<sf::Vector2f> Player::getPreciseBounds() const {
    std::vector<sf::Vector2f> points;
    sf::Transform transform = shape_.getTransform();
    // 获取三角形的三个顶点（玩家飞机是三角形）
    // 注意：由于setPointCount(3)，三角形有三个顶点
    for (size_t i = 0; i < 3; ++i) {
        points.push_back(transform.transformPoint(shape_.getPoint(i)));
    }
    
    return points;
}

void Player::setWindowBounds(const sf::Vector2u& windowSize) {
    windowBounds_ = sf::FloatRect(0, 0, static_cast<float>(windowSize.x), static_cast<float>(windowSize.y));
}

void Player::handleEvent(const sf::Event& event) {
    //1
}

void Player::update(float dt) {
    shooter_.update(sf::seconds(dt));
    
    // 获取鼠标位置（目标位置）
    sf::Vector2i mousePos = sf::Mouse::getPosition();
    targetPosition_.x = static_cast<float>(mousePos.x) - P_SIZE / 2.0f - 470.0f;
    targetPosition_.y = static_cast<float>(mousePos.y) - P_SIZE / 2.0f - 100.0f;
    
    // 边界检查 - 确保目标位置在窗口内
    targetPosition_.x = std::max(0.0f, std::min(targetPosition_.x, windowBounds_.width - P_SIZE));
    targetPosition_.y = std::max(0.0f, std::min(targetPosition_.y, windowBounds_.height - P_SIZE));
    
    // 获取当前位置
    sf::Vector2f currentPosition = shape_.getPosition();
    
    // 使用线性插值 (Lerp) 实现平滑移动
    // 公式: newPosition = current + (target - current) * smoothingFactor * dt
    sf::Vector2f newPosition;
    newPosition.x = currentPosition.x + (targetPosition_.x - currentPosition.x) * smoothingFactor_ * dt;
    newPosition.y = currentPosition.y + (targetPosition_.y - currentPosition.y) * smoothingFactor_ * dt;
    
    // 应用新位置
    shape_.setPosition(newPosition);
}

void Player::draw(sf::RenderWindow& window) const {
    window.draw(shape_);
}

sf::FloatRect Player::getBounds() const {
    return shape_.getGlobalBounds();
}

std::unique_ptr<Bullet> Player::shoot() {
    if (!shooter_.canShoot()) {
        return nullptr;
    }
    // 计算子弹发射位置（从玩家中心顶部发射）
    sf::Vector2f playerPos = shape_.getPosition();
    float bulletStartX = playerPos.x + P_SIZE / 2 - B_RADIUS / 2;  // 居中
    float bulletStartY = playerPos.y;// 从顶部发射
    
    // 创建竖直向上的子弹 (speed_x=0, speed_y=-B_SPEED)
    return shooter_.shoot(bulletStartX, bulletStartY, 0.0f, -B_SPEED);
}