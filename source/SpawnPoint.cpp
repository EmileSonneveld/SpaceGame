#include "SpawnPoint.h"
#include "Gameplay.h"
#include "Enemy.h"

SpawnPoint::SpawnPoint(sf::Vector2f pos) : m_Pos(pos), m_Timer(99)
{
}


SpawnPoint::~SpawnPoint()
{
}

void SpawnPoint::Tick(float dt)
{
	m_Timer += dt;
	if (m_Timer > 20){
		m_Timer = 0;
		if ((Gameplay::getInst().GetPlayerPos() - to_b2Vec2(m_Pos)).Length() < 400)
		{
			auto ennemy = new Enemy(m_Pos);

			Gameplay::getInst().EnqueueAddToList(ennemy);
		}
	}
}