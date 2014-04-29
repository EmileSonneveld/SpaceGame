#include "Turret.h"
#include "Sltn.h"
#include "Gameplay.h"
#include <Box2D\Box2D.h>
#include "main.h"


Turret::Turret(sf::Vector2f pos) :
    BallBase(pos, 5.0f + Random(5)), m_ShootTimer(0), m_isBursting(false), 
    m_burstTimer(0), m_chasingPlayer(false), m_turretSprite()
{
	this->setTexture(Sltn::getInst().GetTexture("resources/blue-sphere_512.png"));
    m_turretSprite.setTexture(Sltn::getInst().GetTexture("resources/bullet.png"));
	Sprite::setScale(Sprite::getScale()*.5f);
}



Turret::~Turret()
{
	Sltn::getInst().EnqueDestroyPhysicsEntity(m_b2Body);
	m_b2Body = nullptr;
}

void Turret::Initialize()
{
	BallBase::CreateBall();
	m_b2Body->GetFixtureList()->SetDensity(0.2f);

	//((UserData*)m_b2Body->GetUserData())->kind = UserData::Turret;
	auto ud = (UserData*)m_b2Body->GetUserData();
	ud->kind = UserData::Ball;
	ud->isCore = false;

	//setFilterGroup(-3); // random number
	//setFilter(UserData::Turret, UserData::Turret | UserData::player | UserData::Turret);
	//setFilter(UserData::Turret, -1 & ~UserData::Turret);
	setFilter(UserData::Ball, ~(UserData::Ball << 1));


	//Gameplay::getInst().StickBodyToWorld(m_b2Body);


}

void Turret::CustomTick(float dt)
{
	m_turretSprite.setPosition(m_b2Body->GetPosition().x, m_b2Body->GetPosition().y);
}
