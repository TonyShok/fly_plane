// Shooter.h
#pragma once
#include "Bullet.h"
#include <SFML/System/Time.hpp>
#include <memory> // for std::unique_ptr
#include <vector>
class Bullet;

class Shooter {
public:
    Shooter(sf::Time fireInterval, int bulletType);
    void update(sf::Time deltaTime); // 更新发射冷却
    bool canShoot() const; // 检查是否可以进行射击
    std::unique_ptr<Bullet> shoot(float startX, float startY, float speedX, float speedY); // 发射子弹
    void setFireInterval(sf::Time interval); // 设置发射间隔
    void additionalCooldown(sf::Time extraTime); // 增加额外冷却时间
private:
    sf::Time mFireInterval; // 发射间隔
    sf::Time mCooldown;     // 当前冷却时间
    int btype = 0; // 子弹类型
};