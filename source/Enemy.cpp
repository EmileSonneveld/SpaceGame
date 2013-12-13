#include "Enemy.h"
#include "sltn.h"
#include "Gameplay.h"
#include <Box2D\Box2D.h>
#include "main.h"


Enemy::Enemy(sf::Vector2f pos) : 
	Ball(pos, 9.0f)
{
	( (UserData*) m_b2Body->GetUserData() )->kind= UserData::Enemy;
	this->setTexture(sltn::getInst().GetTexture("resources/Wheatley.png"), 3);

	Sprite::setScale(Sprite::getScale()*2.0f);
}


Enemy::~Enemy()
{
	sltn::getInst().EnqueDestroyBody(m_b2Body);
	m_b2Body=nullptr;
}

void Enemy::CustomStuff(float dt)
{

	int nrOfFrames= 3; // 19
	auto rectWidth= (float)getTexture()->getSize().x/nrOfFrames;

	auto prevRect = getTextureRect();
	prevRect.left+= rectWidth* (int)(  dt*rectWidth/2 );
	Sprite::setTextureRect(prevRect);

	if( m_b2Body == nullptr ) return;

	//m_b2Body->SetAngularVelocity(-m_b2Body->GetAngle());

	auto moveVector= Gameplay::getInst().GetPlayerPos() - m_b2Body->GetPosition();


	if(abs(moveVector.x)>0.01f || abs(moveVector.y)>0.01f){
		moveVector.Normalize();
		moveVector*=2000.0f;

		assert(m_b2Body);
		m_b2Body->ApplyForceToCenter(moveVector);

	}

}
