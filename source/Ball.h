#pragma once

#include "entityBase.h"

class b2Body;
struct b2Vec2;

class Ball : public entityBase
{
public:
	Ball(sf::Vector2f pos, float radius= 2.0f);
	virtual ~Ball();

	void Tick(float dt);

	virtual void CustomStuff(float dt){} // Inherited object can implement stuff each Tick.
	void setTexture(const sf::Texture& texture, int nrOfFrames=1); // bevat extras

protected:
	//void DoMovement(b2Vec2& moveVector); // modify the vector ans Applys force.
	void SetSpriteFramesNr(float nr){
	}
    float m_lives;
	float m_radius;

private:

};

