#pragma once
#include "entityBase.h"
#include "Ball.h"

class b2Body;

class Player : public Ball
{
public:
	Player(sf::Vector2f pos);
	virtual ~Player();

	//virtual void draw(RenderTarget& target, RenderStates states) const;
	virtual void CustomTick(float dt); // Inherited object can implement stuff each Tick;
	

private:
	float m_AnimationFlow;
	float m_ShootTimer;

	// Disabling default copy constructor and default assignment operator.
	Player(const Ball& yRef);
	Player& operator=(const Player& yRef);
};

