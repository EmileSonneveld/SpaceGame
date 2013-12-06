#pragma once

#include <SFML/Graphics.hpp>
class b2Body;

// Just to have a virtual destructor and sf::Sprite
// So the object can be passed as user data
class entityBase : public sf::Sprite
{
public:
    virtual void Tick(float dt)=0;

    entityBase() : sf::Sprite(), m_b2Body(nullptr){}
    virtual ~entityBase(){}

    b2Body* GetB2Body(){ return m_b2Body; }
    void nullB2Body(){ m_b2Body=nullptr; }

protected:
	b2Body* m_b2Body;
    
};
