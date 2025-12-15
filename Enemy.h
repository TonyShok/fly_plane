#pragma once

#include "GameObject.h"
#include "Bullet.h"
#include "Shooter.h"
#include <SFML/Graphics.hpp>
#include <random>

// ===================================================================
// DERIVED CLASS (Enemy)
//
// This header *declares* the Enemy class. It also
// inherits from GameObject and provides its own overrides.
// ===================================================================
class Enemy : public GameObject {
public:    
    Enemy(float x, float y, float speed);

    // Fulfills the "contract" from GameObject.
    void update(float dt) override;
    void draw(sf::RenderWindow& window) const override;
    sf::FloatRect getBounds() const override;

    // Overrides the 'isAlive' function to use its
    // internal 'alive' flag.
    bool isAlive() const override;
    std::unique_ptr<Bullet> shoot(int type);
    void takeDamage(int damage);
    void addspeed(float deltaSpeed);
    void addcooldown(float extraTime);

private:
    Shooter shooter_;
    sf::CircleShape shape_;
    float speed_;
    int health_;
    bool alive_;
};
