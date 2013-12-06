#pragma once
#include "entityBase.h"

class b2Body;

class Player : public entityBase
{
public:
	Player(sf::Vector2f pos);
	virtual ~Player();

	void Tick(float dt);
	//virtual void draw(RenderTarget& target, RenderStates states) const;

	void setTexture(const sf::Texture& texture, bool resetRect = false); // bevat extras

private:
	float m_radius;
};

