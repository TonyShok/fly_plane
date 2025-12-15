#include "Bullet.h"
#include "config.h"

Bullet::Bullet(float x, float y, float speed_x, float speed_y, int type) : speed_x(speed_x), speed_y(speed_y), alive_(true) {
    shape_.setRadius(B_RADIUS);
    if(type == 0)
        shape_.setFillColor(sf::Color::Yellow);
    else if(type == 1)
        shape_.setFillColor(sf::Color::Cyan);  
    shape_.setPosition({x, y});
}

void Bullet::update(float dt) {
    // Move the bullet downwards
    shape_.move({speed_x * dt, speed_y * dt});

    // Mark as "dead" (ready for removal) if it's off-screen
    sf::FloatRect bounds = shape_.getGlobalBounds();
    if (bounds.top > W_HEIGHT || 
        bounds.top + bounds.height < 0 || 
        bounds.left > W_WIDTH || 
        bounds.left + bounds.width < 0) {
        alive_ = false;
    }
}

void Bullet::draw(sf::RenderWindow& window) const {
    window.draw(shape_);
}
void Bullet::disappear() {
    alive_ = false;
}

sf::FloatRect Bullet::getBounds() const {
    return shape_.getGlobalBounds();
}

bool Bullet::isAlive() const {
    return alive_;
}