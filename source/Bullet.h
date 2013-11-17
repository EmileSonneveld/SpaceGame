#pragma once
#include <SFML/Graphics.hpp>

class b2Body;

class Bullet : public sf::Sprite
{
public:
	Bullet(sf::Vector2f pos, float angle);
	virtual ~Bullet();

	void Tick(float dt);

	void setTexture(const sf::Texture& texture, bool resetRect = false); // bevat extras
	b2Body* GetB2Body(){return m_b2Body;}
private:
	b2Body* m_b2Body;
    float m_damage;
	float m_radius;
    float m_lifeTime;
};

