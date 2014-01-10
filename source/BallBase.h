#pragma once

#include "entityBase.h"

class b2Body;
class b2World;
struct b2Vec2;

class BallBase : public entityBase
{
public:
	BallBase(sf::Vector2f pos, float radius = 2.0f);
	virtual ~BallBase();

	void Tick(float dt);

	virtual void CustomTick(float /*dt*/){  } // Inherited object can implement stuff each Tick.
	void setTexture(const sf::Texture& texture, bool calculateFramesFromSquare = true); // bevat extras
	void DoDammage(float dammage);
	void Shoot(const b2Vec2 target, bool ai = true);

	static float semiGlobal_minDistanceSquared; // = 50
	static float semiGlobal_minDistance;

protected:
	//void DoMovement(b2Vec2& moveVector); // modify the vector ans Applys force.
	//void SetSpriteFramesNr(float nr){	}

	void CreateBall(); // Get called by the endobject
	float m_lives;
	float m_radius;
	int m_nrOfFrames;

private:
	// Disabling default copy constructor and default assignment operator.
	BallBase(const BallBase& yRef);
	BallBase& operator=(const BallBase& yRef);
};

