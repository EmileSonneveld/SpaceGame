#include "Player.h"
#include "sltn.h"
#include <Box2D\Box2D.h>
#include "main.h"


Player::Player(sf::Vector2f pos) :
Ball(pos, 4.0f), m_AnimationFlow(0)
{
	((UserData*)m_b2Body->GetUserData())->kind = UserData::player;
	this->setTexture(sltn::getInst().GetTexture("resources/Wheatley.png"), 3);

	setFilterGroup(-546541); // random number

	Sprite::setScale(Sprite::getScale()*2.0f);
}


Player::~Player()
{
	sltn::getInst().EnqueDestroyBody(m_b2Body);
	m_b2Body = nullptr;
}

void Player::CustomStuff(float dt)
{

	int nrOfFrames = 3; // 19
	auto rectWidth = (float)getTexture()->getSize().x / nrOfFrames;

	m_AnimationFlow += dt;

	auto prevRect = getTextureRect();
	prevRect.left = (int) ( ((int)(m_AnimationFlow*rectWidth / rectWidth))*rectWidth );
	Sprite::setTextureRect(prevRect);

	if (m_b2Body == nullptr) return;

	//m_b2Body->SetAngularVelocity(-m_b2Body->GetAngle());

	auto moveVector = b2Vec2(0, 0); //sf::Vector2f();
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) moveVector.x -= 1;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) moveVector.x += 1;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) moveVector.y -= 1;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) moveVector.y += 1;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) moveVector.x -= 1;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) moveVector.x += 1;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) moveVector.y -= 1;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) moveVector.y += 1;

	if (abs(moveVector.x) > 0.01f || abs(moveVector.y) > 0.01f){
		moveVector.Normalize();
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
			moveVector *= 5.0f;
		moveVector *= 800.0f;

		assert(m_b2Body);
		m_b2Body->ApplyForceToCenter(moveVector);

	}

}

