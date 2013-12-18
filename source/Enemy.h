#pragma once
#include "entityBase.h"
#include "Ball.h"

class b2Body;

class Enemy : public Ball
{
public:
	Enemy(sf::Vector2f pos);
	virtual ~Enemy();

	//virtual void draw(RenderTarget& target, RenderStates states) const;
	virtual void CustomStuff(float dt); // Inherited object can implement stuff each Tick;
	

private:
	//b2Vec2 target;
};

