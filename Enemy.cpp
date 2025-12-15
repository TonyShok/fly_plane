#include "Enemy.h"
#include "config.h"
#include "Bullet.h"
#include "Shooter.h"
#include <random>

Enemy::Enemy(float x, float y, float speed) : health_(E_HEALTH) , speed_(speed), alive_(true), 
    shooter_(sf::seconds(E_FIRE_COOLDOWN),1)
{
    shape_.setPointCount(3);
    shape_.setRadius(E_RADIUS);
    shape_.setFillColor(sf::Color::Red);
    shape_.setPosition({x, y});
    shape_.setRotation(180);
}

void Enemy::update(float dt) {
    shooter_.update(sf::seconds(dt) * 1.0f);
    // Move the enemy downwards
    shape_.move({0, speed_ * dt});

    // Mark as "dead" (ready for removal) if it's off-screen
    if (shape_.getPosition().y > W_HEIGHT) {
        alive_ = false;
    }
    if(health_ <=0 ){
        alive_ = false;
    }
}
void Enemy::addspeed(float deltaSpeed){
    speed_ += deltaSpeed;
}

void Enemy::draw(sf::RenderWindow& window) const {
    window.draw(shape_);
}

sf::FloatRect Enemy::getBounds() const {
    return shape_.getGlobalBounds();
}

void Enemy::takeDamage(int damage) {
    health_ -= damage;
    if (health_ <= 0) {
        alive_ = false;
    }
}
void Enemy::addcooldown(float extraTime){
    shooter_.additionalCooldown(sf::seconds(extraTime));
}

std::unique_ptr<Bullet> Enemy::shoot(int type) {
    if (shooter_.canShoot()) {
        float startX = shape_.getPosition().x - E_RADIUS / 0.84f;
        float startY = shape_.getPosition().y - E_RADIUS / 2.0f;
        float speedX = 0.5f * type * B_SPEED; // Adjust horizontal speed based on type
        float speedY = (1 - 0.13 * abs(type)) * B_SPEED; // Enemy bullets go downwards
        
        return shooter_.shoot(startX, startY, speedX, speedY);
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<float> dist(0.0f, 0.5f);
        float randomCooldown = dist(gen);
    }
    return nullptr;
}

bool Enemy::isAlive() const {
    return alive_;
}
