#pragma once
#include "entityBase.h"
#include "BallBase.h"

class b2Body;

class Player : public BallBase
{
public:
	Player(sf::Vector2f pos);
	virtual ~Player();
	virtual void Initialize();

	//virtual void draw(RenderTarget& target, RenderStates states) const;
	virtual void CustomTick(float dt); // Inherited object can implement stuff each Tick;
	

private:
	float m_AnimationFlow;
	float m_ShootTimer;

	// Disabling default copy constructor and default assignment operator.
	Player(const BallBase& yRef);
	Player& operator=(const Player& yRef);
};

