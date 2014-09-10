#include "Bullet.h"
#include "Player.h"
#include "Gameplay.h"
#include "SpriteAnimation.h"
#include "Sltn.h"
#include <Box2D/Box2D.h>

FMOD::Sound* Bullet::m_sound = nullptr;

Bullet::Bullet(sf::Vector2f pos, float angle, bool useSound) : entityBase(), m_radius(2.0f), m_damage(20.0), m_lifeTime(5)
{
	setPosition(pos);
	if (useSound){
		if (m_sound == nullptr) m_sound = Sltn::getInst().getSound("./resources/PY7.wav");
		Sltn::getInst().playSound(m_sound);
	}

	b2BodyDef bd;
	bd.type = b2_dynamicBody;
	bd.position.Set(pos.x, pos.y);
	bd.awake = false;
	auto ud = new UserData(this);
	//ud->creator = this;
	ud->kind = UserData::bullet;
	bd.userData = ud;
	m_b2Body = Sltn::getInst().m_world->CreateBody(&bd);
	m_b2Body->SetTransform(to_b2Vec2(pos), angle); // b2Vec2()

	b2CircleShape shape;
	shape.m_radius = m_radius / 2;

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &shape;
	//fixtureDef.friction = 0;
	fixtureDef.density = 1;
	fixtureDef.restitution = 0.5;
	//fixtureDef.filter.groupIndex = 0;
	m_b2Body->CreateFixture(&fixtureDef);


	setTexture(Sltn::getInst().GetTexture("resources/bullet.png"));

}


Bullet::~Bullet()
{

}

void Bullet::DestroyBody()
{
	Sltn::getInst().EnqueDestroyPhysicsEntity(m_b2Body);
	m_b2Body = nullptr;
}

void Bullet::Tick(float dt) // 0.0166
{

	m_lifeTime -= dt;

	if (m_b2Body){
		auto pos = m_b2Body->GetPosition();
		setPosition(pos.x, pos.y);
		float angle = 180.0f / 3.1415f*m_b2Body->GetTransform().q.GetAngle();
		Sprite::setRotation(angle);

		if (m_lifeTime < 0){
			DestroyBody();
			m_lifeTime = 2;
		}

		if (rand() % 50 < 1){
			auto spriteAnimation = new SpriteAnimation(getPosition(), false);
			Gameplay::getInst().AddToList(spriteAnimation);
		}

	}
	else{
		if (m_lifeTime < 0){
			Gameplay::getInst().EnqueueRemoveFromList(this);
			m_lifeTime = 2;
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
