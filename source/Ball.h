#pragma once

#include "entityBase.h"

class b2Body;

class Ball : public entityBase
{
public:
	Ball(sf::Vector2f pos);
	virtual ~Ball();

	void Tick(float dt);

	void setTexture(const sf::Texture& texture, bool resetRect = false); // bevat extras
	b2Body* GetB2Body(){return m_b2Body;}
private:
	b2Body* m_b2Body;
	//sf::Vector2f m_Velocity;
    float m_lives;
	float m_radius;
};

