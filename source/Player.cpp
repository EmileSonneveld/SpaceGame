#include "Player.h"
#include "sltn.h"
#include <Box2D\Box2D.h>
#include "main.h"
#include "Gameplay.h"


Player::Player(sf::Vector2f pos) :
Ball(pos, 4.0f), m_AnimationFlow(0), m_ShootTimer(0)
{
	((UserData*)m_b2Body->GetUserData())->kind = UserData::player;
	this->setTexture(sltn::getInst().GetTexture("resources/Wheatley.png"));

	setFilterGroup(-2); // random number

	Sprite::setScale(Sprite::getScale()*2.0f);
}


Player::~Player()
{
	sltn::getInst().EnqueDestroyPhysicsEntity(m_b2Body);
	m_b2Body = nullptr;
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


	auto worldPos = sltn::getInst().GetMousePos();
	Gameplay& gp = Gameplay::getInst();
	if (m_ShootTimer > 0.2f && sf::Mouse::isButtonPressed(sf::Mouse::Left)){
		m_ShootTimer = 0;



		// - sf::Vector2i( viewrect.x, viewrect.y);
		auto force = to_b2Vec2(worldPos) - b2Vec2(gp.GetPlayerPos().x, gp.GetPlayerPos().y);
		auto len = force.Length();
		force.x /= len;
		force.y /= len;
		//force*=10.0f;

		auto bullet = new Bullet(to_Vector2(gp.GetPlayerPos() + force)
			, atan2(force.y, force.x), true);
		bullet->setFilterGroup(this->getFilterGroup());
		force *= 65000.0f;
		bullet->GetB2Body()->ApplyForceToCenter(force);
		//m_bulletVec.push_front(bullet);
		gp.EnqueueAddToList(bullet);
		//m_bulletVec.remove(bullet);
	}
	if (m_ShootTimer > 0.001f && sf::Mouse::isButtonPressed(sf::Mouse::Middle))
	{
		m_ShootTimer = 0;

		// - sf::Vector2i( viewrect.x, viewrect.y);
		auto force = to_b2Vec2(worldPos) - b2Vec2(gp.GetPlayerPos().x, gp.GetPlayerPos().y);
		auto len = force.Length();
		force.x /= len;
		force.y /= len;
		//force*=10.0f;

		auto bullet = new Bullet(getPosition() + to_Vector2(force)
			, atan2(force.y, force.x));
		bullet->setFilterGroup(getFilterGroup());
		force *= 45000.0f;
		bullet->GetB2Body()->ApplyForceToCenter(force);
		gp.EnqueueAddToList(bullet);
		//m_bulletVec.push_front(bullet);
		//m_bulletVec.remove(bullet);
	}
	if (m_ShootTimer > 0.05f && sf::Mouse::isButtonPressed(sf::Mouse::Right)){
		m_ShootTimer = 0;

		auto ball = new Ball(worldPos);
		ball->setTexture(sltn::getInst().GetTexture("resources/blue-sphere_512.png"));
		//m_Balls.push_back(ball);
		gp.EnqueueAddToList(ball);
		ConnectTry(this->GetB2Body(), ball->GetB2Body());
		gp.ConnectWithOthers(ball);
		// for (auto ball1 : m_Balls){
		// 	if (ball1 == nullptr) continue;
		// 	//if( ( (UserData*)ball1.GetB2Body()->GetUserData() )->isConectedToCluster == false ) continue;
		// 	ConnectTry(ball1->GetB2Body(), m_Balls.back()->GetB2Body());
		// }

		//b2Body* bodyA= m_player.GetB2Body(); // m_playe
		//b2Body* bodyB= m_Balls.back()->GetB2Body(); // ball

	}
}

