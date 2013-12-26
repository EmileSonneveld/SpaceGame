#include "Bullet.h"
#include "Player.h"
#include "Gameplay.h"
#include "SpriteAnimation.h"
#include "sltn.h"
#include <Box2D\Box2D.h>

FMOD::Sound* Bullet::m_sound = nullptr;

Bullet::Bullet(sf::Vector2f pos, float angle, bool useSound) : entityBase(), m_radius(2.0f), m_damage(40.0), m_lifeTime(5)
{
	setPosition(pos);
	if (useSound){
		if (m_sound == nullptr) m_sound = sltn::getInst().getSound("./resources/PY7.wav");
		sltn::getInst().playSound(m_sound);
	}

	b2BodyDef bd;
	bd.type = b2_dynamicBody;
	bd.position.Set(pos.x, pos.y);
	bd.awake = false;
	auto ud = new UserData();
	ud->creator = this;
	ud->kind = UserData::bullet;
	bd.userData = ud;
	m_b2Body = sltn::getInst().m_world->CreateBody(&bd);
	m_b2Body->SetTransform(to_b2Vec2(pos), angle); // b2Vec2()
	b2CircleShape shape;
	shape.m_radius = m_radius / 2;
	//b2PolygonShape shape;
	//shape.SetAsBox(m_radius/2,m_radius/2);
	m_b2Body->CreateFixture(&shape, 1.0f);

	setTexture(sltn::getInst().GetTexture("resources/bullet.png"));

}


Bullet::~Bullet()
{
	sltn::getInst().EnqueDestroyBody(m_b2Body);
	m_b2Body = nullptr;
}

void Bullet::DestroyBody()
{
	sltn::getInst().EnqueDestroyBody(m_b2Body);
	m_b2Body = nullptr;
}

void Bullet::Tick(float dt) // 0.0166
{


	if (m_b2Body){
		auto pos = m_b2Body->GetPosition();
		setPosition(pos.x, pos.y);
		float angle = 180.0f / 3.1415f*m_b2Body->GetTransform().q.GetAngle();
		Sprite::setRotation(angle);

		m_lifeTime -= dt;
		if (m_lifeTime < 0)
			DestroyBody();

		if (rand() % 50 < 1){
			auto spriteAnimation = new SpriteAnimation(getPosition(), false);
			Gameplay::getInst().Add(spriteAnimation);
		}

	}
}

void Bullet::setTexture(const sf::Texture& texture, bool resetRect)
{
	sf::Sprite::setTexture(texture, resetRect);

	float scale = m_radius / getTexture()->getSize().x;
	setOrigin((float)texture.getSize().x / 2U, (float)texture.getSize().y / 2U);
	setScale(scale, scale);
	//sf::Transformable::setOrigin(getTexture()->getSize()/2u); // todo: proper center
}