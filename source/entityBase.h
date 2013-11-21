#pragma once

#include <SFML/Graphics.hpp>

// Just to have a virtual destructor and sf::Sprite
// So the object can be passed as user data
class entityBase : public sf::Sprite
{
public:
    virtual void Tick(float dt)=0;

    entityBase() : sf::Sprite(){}
    virtual ~entityBase(){}

};
