#include "Ball.h"
#include "Sltn.h"
#include "Gameplay.h"
#include <Box2D\Box2D.h>
#include "main.h"


Ball::Ball(sf::Vector2f pos) :
BallBase(pos, 5.0f), m_ShootTimer(0), m_isBursting(false), m_burstTimer(0), m_chasingPlayer(false)
{
	this->setTexture(Sltn::getInst().GetTexture("resources/blue-sphere_512.png"));
	Sprite::setScale(Sprite::getScale()*.5f);
}



Ball::~Ball()
{
	Sltn::getInst().EnqueDestroyPhysicsEntity(m_b2Body);
	m_b2Body = nullptr;
}

void Ball::Initialize()
{
	BallBase::CreateBall();
	m_b2Body->GetFixtureList()->SetDensity(0.2f);

	//((UserData*)m_b2Body->GetUserData())->kind = UserData::Ball;
	auto ud = (UserData*)m_b2Body->GetUserData();
	ud->kind = UserData::Ball;
	ud->isCore = false;

	//setFilterGroup(-3); // random number
	//setFilter(UserData::Ball, UserData::ball | UserData::player | UserData::Ball);
	//setFilter(UserData::Ball, -1 & ~UserData::Ball);
	setFilter(UserData::Ball, ~(UserData::Ball << 1));


	Gameplay::getInst().StickBodyToWorld(m_b2Body);


}

void Ball::CustomTick(float dt)
{
	// do nothing
}
