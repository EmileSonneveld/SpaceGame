#pragma once
#include "entityBase.h"

#include "../fmod_inc/fmod.hpp"
class b2Body;
//class FMOD::Sound;

class Bullet: public entityBase
{
public:
	Bullet(sf::Vector2f pos, float angle, bool useSound= false);
	virtual ~Bullet();

	void Tick(float dt);

    void DestroyBody();

	void setTexture(const sf::Texture& texture, bool resetRect = false); // bevat extras
	
private:
    float m_damage;
	float m_radius;
    float m_lifeTime;

	static FMOD::Sound* m_sound; // Shooting sound
};

