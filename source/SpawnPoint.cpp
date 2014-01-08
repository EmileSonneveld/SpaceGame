#include "SpawnPoint.h"
#include "Gameplay.h"
#include "Enemy.h"

SpawnPoint::SpawnPoint(sf::Vector2f pos) : m_Pos(pos), m_Timer(999)
{
}


SpawnPoint::~SpawnPoint()
{
}

void SpawnPoint::Tick(float dt)
{
	m_Timer += dt;
	if (m_Timer > 50){
		m_Timer = 0;
		auto ennemy = new Enemy(m_Pos);
		ennemy->GetB2Body()->ApplyForceToCenter(b2Vec2(rand() % 2000 - 1000, rand() % 2000 - 1000));
		Gameplay::getInst().EnqueueAddToList(ennemy);
	}
}