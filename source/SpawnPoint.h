#pragma once

#include <Box2D/Common/b2Math.h>
#include <SFML/Graphics.hpp>

class SpawnPoint
{
public:
	SpawnPoint(sf::Vector2f pos);
	virtual ~SpawnPoint();

	void Tick(float dt);
private:
	sf::Vector2f m_Pos;
	float m_Timer;
};

