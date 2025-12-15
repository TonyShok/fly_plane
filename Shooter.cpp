// Shooter.cpp
#include "Shooter.h"
#include "Bullet.h"

Shooter::Shooter(sf::Time fireInterval, int bulletType)
    : mFireInterval(fireInterval), mCooldown(sf::Time::Zero) , btype(bulletType){
        
    }

void Shooter::update(sf::Time deltaTime) {
    if (mCooldown > sf::Time::Zero) {
        mCooldown -= deltaTime;
    }
}

bool Shooter::canShoot() const {
    return mCooldown <= sf::Time::Zero;
}

std::unique_ptr<Bullet> Shooter::shoot(float startX, float startY, float speedX, float speedY) {
    if (!canShoot()) {
        return nullptr; 
    }
    mCooldown = mFireInterval; // 重置冷却时间
    // 创建并返回一颗新子弹
    return std::make_unique<Bullet>(startX, startY, speedX, speedY, btype);
}

void Shooter::additionalCooldown(sf::Time extraTime) {
    mCooldown += extraTime;
}

void Shooter::setFireInterval(sf::Time interval) {
    mFireInterval = interval;
}