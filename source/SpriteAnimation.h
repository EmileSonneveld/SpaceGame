#pragma once

#include "entityBase.h"

class b2Body;

class SpriteAnimation : public entityBase
{
public:
	SpriteAnimation(sf::Vector2f pos, bool looping= true);
	virtual ~SpriteAnimation();

	void Tick(float dt);

	void setTexture(const sf::Texture& texture, bool resetRect = false); // bevat extras
private:
	bool m_looping;
	float m_radius;
	int m_numImages;
	float m_rectWidth;

};

