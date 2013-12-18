#pragma once
#include "entityBase.h"


class b2Body;

class Bullet: public entityBase
{
public:
	Bullet(sf::Vector2f pos, float angle);
	virtual ~Bullet();

	void Tick(float dt);

    void DestroyBody();

	void setTexture(const sf::Texture& texture, bool resetRect = false); // bevat extras
	
private:
    float m_damage;
	float m_radius;
    float m_lifeTime;
};

