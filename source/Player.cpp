#include "Player.h"
#include "sltn.h"
#include <Box2D\Box2D.h>
#include "main.h"
#include "Gameplay.h"
#include "Bullet.h"

Player::Player(sf::Vector2f pos) :
Ball(pos, 4.0f), m_AnimationFlow(0), m_ShootTimer(0)
{
	((UserData*)m_b2Body->GetUserData())->kind = UserData::player;
	this->setTexture(sltn::getInst().GetTexture("resources/Wheatley.png"));

	m_b2Body->GetFixtureList()->SetFriction(0.9f);

	setFilter(UserData::player, ~(UserData::player << 1));

	Sprite::setScale(Sprite::getScale()*2.0f);
}


Player::~Player()
{
}

void Player::CustomStuff(float dt)
{
	m_ShootTimer += dt;



	int nrOfFrames = 3; // 19
	auto rectWidth = (float)getTexture()->getSize().x / nrOfFrames;

	m_AnimationFlow += dt;

	auto prevRect = getTextureRect();
	prevRect.left = (int)(((int)(m_AnimationFlow*rectWidth / rectWidth))*rectWidth);
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
		moveVector *= 5000.0f;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
			moveVector *= 2.0f;

		assert(m_b2Body);
		m_b2Body->ApplyForceToCenter(moveVector);

	}


	auto targetPos = sltn::getInst().GetMousePos();
	Gameplay& gp = Gameplay::getInst();

	if (m_ShootTimer > 0.05f && sf::Mouse::isButtonPressed(sf::Mouse::Left))	{
		m_ShootTimer = 0;

		Shoot(to_b2Vec2(targetPos), false);
	}

	if (m_ShootTimer > 0.2f && sf::Mouse::isButtonPressed(sf::Mouse::Middle)){
		m_ShootTimer = 0;

		// Make a bullet that shoot against everything
		auto force = to_b2Vec2(targetPos) - m_b2Body->GetPosition();
		auto len = force.Length();
		force.x /= len;  force.y /= len;
		//force*=10.0f;

		auto bullet = new Bullet(to_Vector2(m_b2Body->GetPosition() + force)
			, atan2(force.y, force.x), true);
		//bullet->setFilterGroup(this->getFilterGroup());
		//bullet->setFilter(UserData::Enemy << 1, -1);
		force *= 65000.0f;
		bullet->GetB2Body()->ApplyForceToCenter(force);

		gp.EnqueueAddToList(bullet);
	}

	if (m_ShootTimer > 0.2f && sf::Mouse::isButtonPressed(sf::Mouse::Right)){
		m_ShootTimer = 0;

		auto ball = new Ball(targetPos);
		//ball->setTexture(sltn::getInst().GetTexture("resources/blue-sphere_512.png"));
		ball->setFilter(UserData::player, -1 & ~UserData::player);
		gp.EnqueueAddToList(ball);
		 
		//gp.ConnectWithOthers(ball);
		ConnectTry(this->GetB2Body(), ball->GetB2Body());

		// for (auto ball1 : m_Balls){
		// 	if (ball1 == nullptr) continue;
		// 	//if( ( (UserData*)ball1.GetB2Body()->GetUserData() )->isConectedToCluster == false ) continue;
		// 	ConnectTry(ball1->GetB2Body(), m_Balls.back()->GetB2Body());
		// }

	}
}

