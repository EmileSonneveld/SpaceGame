#include "Enemy.h"
#include "sltn.h"
#include "Gameplay.h"
#include <Box2D\Box2D.h>
#include "main.h"


Enemy::Enemy(sf::Vector2f pos) :
Ball(pos, 5.0f), m_ShootTimer(0), m_isBursting(false), m_burstTimer(0)
{
	((UserData*)m_b2Body->GetUserData())->kind = UserData::Enemy;
	this->setTexture(sltn::getInst().GetTexture("resources/Wheatley.png"), 3);

	setFilterGroup(-112234); // random number

	Sprite::setScale(Sprite::getScale()*2.0f);
}
 


Enemy::~Enemy()
{
	sltn::getInst().EnqueDestroyPhysicsEntity(m_b2Body);
	m_b2Body = nullptr;
}
 

void Enemy::CustomStuff(float dt)
{
	m_ShootTimer += dt;
	m_burstTimer += dt;


	Gameplay& gp = Gameplay::getInst();

	if (m_isBursting){
		if (m_burstTimer > 0.5) { m_isBursting = !m_isBursting; m_burstTimer = 0; }
		if (m_ShootTimer > 0.05){
			m_ShootTimer = 0;



			// - sf::Vector2i( viewrect.x, viewrect.y);
			auto force = gp.GetPlayerPos() - m_b2Body->GetPosition();
			auto len = force.Length();
			force.x /= len;
			force.y /= len;
			//force*=10.0f;

			auto bullet = new Bullet(to_Vector2(m_b2Body->GetPosition() + force)
				, atan2(force.y, force.x), true);
			bullet->setFilterGroup(this->getFilterGroup());
			force *= 65000.0f;
			bullet->GetB2Body()->ApplyForceToCenter(force);
			//m_bulletVec.push_front(bullet);
			gp.EnqueueAddToList(bullet);
			//m_bulletVec.remove(bullet);
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
	 

}
