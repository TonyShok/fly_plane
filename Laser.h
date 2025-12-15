#pragma once // Standard header guard, prevents multiple includes.

#include <SFML/Graphics.hpp>
#include "GameObject.h"
class Laser : public GameObject {
    public:
        Laser(float x, float y, float angle);
        void update(float dt) override;
        void draw(sf::RenderWindow& window) const override;
        sf::FloatRect getBounds() const override;
        bool isAlive() const override { return alive_; }
        bool isActive() const { return active_; }
        std::vector<sf::Vector2f> getTransformedPoints() const;

    private:
        sf::RectangleShape shape_;
        float width;
        bool grow_ = true;
        bool active_ = false;
        bool alive_ = true;
};