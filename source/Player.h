#pragma once
#include "entityBase.h"

class b2Body;
class MainClass;

class Player : public entityBase
{
public:
	Player(sf::Vector2f pos);
	virtual ~Player();

	void Tick(float dt);
	//virtual void draw(RenderTarget& target, RenderStates states) const;

	void setTexture(const sf::Texture& texture, bool resetRect = false); // bevat extras
	b2Body* GetB2Body(){return m_b2Body;}

private:

	b2Body* m_b2Body;
	float m_radius;
};

