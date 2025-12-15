#pragma once // Standard header guard, prevents multiple includes.

#include <SFML/Graphics.hpp>

class GameObject {
public:
    // A virtual destructor is crucial for polymorphism.
    // It ensures that when we delete a derived class (like Enemy)
    // through a base class pointer (GameObject*), the correct
    // (Enemy::~Enemy) is called.
    virtual ~GameObject() = default;

    // A virtual function for handling events.
    // We give it an empty body {} so derived classes *can*
    // override it, but don't *have* to. (Player will, Enemy won't).
    virtual void handleEvent(const sf::Event& event) {}

    // A 'pure virtual function' (because of '= 0').
    // This makes GameObject an abstract class.
    // It *forces* any child class (like Player or Enemy)
    // to write its own 'update' function.
    virtual void update(float dt) = 0;

    // Another pure virtual function for drawing.
    virtual void draw(sf::RenderWindow& window) const = 0;

    // Another pure virtual function for collision detection.
    virtual sf::FloatRect getBounds() const = 0;
    
    // Not pure virtual, provides a default implementation
    // that derived classes can (but don't have to) override.
    virtual bool isAlive() const { return true; } 
};