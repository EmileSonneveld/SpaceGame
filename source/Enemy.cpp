#include "Enemy.h"
#include "sltn.h"
#include "Gameplay.h"
#include <Box2D\Box2D.h>
#include "main.h"


Enemy::Enemy(sf::Vector2f pos) :
Ball(pos, 5.0f), m_ShootTimer(0), m_isBursting(false), m_burstTimer(0), m_chasingPlayer(false)
{
	//((UserData*)m_b2Body->GetUserData())->kind = UserData::Enemy;
	auto ud = (UserData*)m_b2Body->GetUserData();
	ud->kind = UserData::Enemy;
	ud->isCore = true;

	this->setTexture(sltn::getInst().GetTexture("resources/Wheatley.png"));
	
	//setFilterGroup(-3); // random number
	//setFilter(UserData::Enemy, UserData::ball | UserData::player | UserData::Enemy);
	//setFilter(UserData::Enemy, -1 & ~UserData::Enemy);
	setFilter(UserData::Enemy, ~(UserData::Enemy << 1));
	Sprite::setScale(Sprite::getScale()*2.0f);

	Gameplay::getInst().MakeCircle(pos, 6, 3);
	Gameplay::getInst().MakeCircle(pos, 7, 3);
}



Enemy::~Enemy()
{
	sltn::getInst().EnqueDestroyPhysicsEntity(m_b2Body);
	m_b2Body = nullptr;
	Gameplay::getInst().AddKill();
}



void Enemy::CustomTick(float dt)
{
	if (!m_chasingPlayer){
		auto force = Gameplay::getInst().GetPlayerPos() - m_b2Body->GetPosition();
		auto len = force.Length();
		if (len < 100) m_chasingPlayer = true;
		return;
	}

	m_ShootTimer += dt;
	m_burstTimer += dt;



	if (m_isBursting){
		if (m_burstTimer > 0.5) { m_isBursting = !m_isBursting; m_burstTimer = 0; }
		if (m_ShootTimer > 0.05){
			m_ShootTimer = 0;

			Shoot(Gameplay::getInst().GetPlayerPos());

		}
	}
	else{
		if (m_burstTimer > 1) { m_isBursting = !m_isBursting; m_burstTimer = 0; }
		//f (m_ShootTimer > 5){
		//	m_isBursting = true;
		//
	}


	int nrOfFrames = 3; // 19
	auto rectWidth = (float)getTexture()->getSize().x / nrOfFrames;

	auto prevRect = getTextureRect();
	prevRect.left += (int)(rectWidth* (int)(dt*rectWidth*0.01));
	Sprite::setTextureRect(prevRect);

	if (m_b2Body == nullptr) return;

	//m_b2Body->SetAngularVelocity(-m_b2Body->GetAngle());

	auto moveVector = Gameplay::getInst().GetPlayerPos() - m_b2Body->GetPosition();


	if (abs(moveVector.x) > 0.01f || abs(moveVector.y) > 0.01f){
		moveVector.Normalize();
		moveVector *= 1000.0f;

		assert(m_b2Body);
		m_b2Body->ApplyForceToCenter(moveVector);

	}

}
