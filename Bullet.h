#pragma once

#include "GameObject.h"
#include "Shooter.h"
#include <SFML/Graphics.hpp>

// ===================================================================
// DERIVED CLASS (bullet)
//
// This header *declares* the bullet class. It also
// inherits from GameObject and provides its own overrides.
// ===================================================================
class Bullet : public GameObject {
public:    
    Bullet(float x, float y, float speed_x, float speed_y, int type);

    // Fulfills the "contract" from GameObject.
    void update(float dt) override;
    void draw(sf::RenderWindow& window) const override;
    sf::FloatRect getBounds() const override;

    // Overrides the 'isAlive' function to use its
    // internal 'alive' flag.
    bool isAlive() const override;
    void disappear();

private:
    sf::CircleShape shape_;
    float speed_x;
    float speed_y;
    bool alive_;
};