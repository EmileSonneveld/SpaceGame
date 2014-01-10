#pragma once
#include "entityBase.h"
#include "Ball.h"

class b2Body;

class Enemy : public Ball
{
public:
	Enemy(sf::Vector2f pos);
	virtual ~Enemy();

	virtual void Initialize();

	//virtual void draw(RenderTarget& target, RenderStates states) const;
	virtual void CustomTick(float dt); // Inherited object can implement stuff each Tick;

private:
	//b2Vec2 target;
	float m_ShootTimer;
	bool m_isBursting;
	float m_burstTimer;

	bool m_chasingPlayer;
};

