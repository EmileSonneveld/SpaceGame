#pragma once
#include "entityBase.h"
#include "BallBase.h"

class b2Body;

class Ball : public BallBase
{
public:
	Ball(sf::Vector2f pos);
	virtual ~Ball();
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

