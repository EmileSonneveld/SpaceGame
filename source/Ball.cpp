#include "Ball.h"
#include "Player.h"
#include "Bullet.h"
#include "sltn.h"
#include "Gameplay.h"
#include <Box2D\Box2D.h>
#include <Box2D/Dynamics/b2Fixture.h>
#include <math.h>

//__declspec(dllimport) b2Body * __cdecl b2World::CreateBody(b2BodyDef const *);
//#pragma import sfml-graphics-d-2.lib
// #pragma comment(lib, "sfml-graphics-d-2.lib")

float Ball::semiGlobal_minDistanceSquared = 50.0f;
float Ball::semiGlobal_minDistance = sqrt(semiGlobal_minDistanceSquared);

Ball::Ball(sf::Vector2f pos, float radius) : entityBase(), m_lives(100.0f), m_radius(radius), m_nrOfFrames(1)
{
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(pos.x, pos.y);
	if (!bodyDef.position.IsValid()) return;
	bodyDef.awake = false;
	auto userData = new UserData(this);
	//userData->creator = this;
	userData->kind = UserData::ball;
	bodyDef.userData = userData;
	m_b2Body = sltn::getInst().m_world->CreateBody(&bodyDef);

	b2CircleShape shape;	shape.m_radius = m_radius / 2;
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &shape;
	//fixtureDef.friction = 0;
	fixtureDef.density = 1;
	//fixtureDef.filter.groupIndex = 0;
	m_b2Body->CreateFixture(&fixtureDef);

	//b2Filter filter;
	//filter.categoryBits |= UserData::ball;
	//filter.maskBits = UserData::ball | UserData::player | UserData::Enemy;
	//setFilter(UserData::player, -1 & ~(UserData::ball | UserData::player) );

	Sprite::setPosition(pos.x, pos.y);
	if (getTexture()==nullptr)
		setTexture(sltn::getInst().GetTexture("resources/blue-sphere_512.png")); //Exp_yellow_maya blue-sphere_512
}


Ball::~Ball()
{
}



void Ball::Tick(float dt) // 0.0166
{
	CustomStuff(dt);


	if (m_b2Body) // Sync body to sprite
	{
		auto pos = m_b2Body->GetPosition();
		Sprite::setPosition(pos.x, pos.y);

		float angle = 180.0f / b2_pi*m_b2Body->GetTransform().q.GetAngle();
		Sprite::setRotation(angle);

		auto fg = this->getFilterGroup();
		int left = fg * getTexture()->getSize().y;
		Sprite::setTextureRect(sf::IntRect(left, 0, getTexture()->getSize().y, getTexture()->getSize().y));
	}
}



void Ball::setTexture(const sf::Texture& texture, bool calculateFramesFromSquare)
{
	Sprite::setTexture(texture, false);
	if (calculateFramesFromSquare){
		float tmp = (float)getTexture()->getSize().x / (float)getTexture()->getSize().y;
		assert(std::fmodf(tmp, 1) < 0.01); // , "bad texture size, must be strip of squares"

		int nrOfFrames = (int)std::floor(tmp);
		m_nrOfFrames = (int)nrOfFrames;
		int rectWidth = getTexture()->getSize().x / nrOfFrames;

		float scale = m_radius / getTexture()->getSize().y;
		Sprite::setOrigin((float)texture.getSize().y / 2.0f, (float)texture.getSize().y / 2.0f);
		Sprite::setScale(scale, scale);
		int left = 0 * rectWidth;
		Sprite::setTextureRect(sf::IntRect(left, 0, rectWidth, getTexture()->getSize().y));
	}
	else{
		float scale = m_radius / getTexture()->getSize().y;
		Sprite::setOrigin((float)texture.getSize().x / 2.0f, (float)texture.getSize().y / 2.0f);
		Sprite::setScale(scale, scale);
	}
}



void Ball::DoDammage(float dammage){
	m_lives -= dammage;
	if (m_lives < 0) Gameplay::getInst().EnqueueRemoveFromList(this);
}



void Ball::Shoot(const b2Vec2 target, bool ai)
{

	Gameplay& gp = Gameplay::getInst();

	auto force = target - m_b2Body->GetPosition();
	auto len = force.Length();
	if (ai && len > 100) return; // shoot only is in range

	force.x /= len;  force.y /= len;
	//force*=10.0f;

	auto bullet = new Bullet(to_Vector2(m_b2Body->GetPosition() + force)
		, atan2(force.y, force.x), false);
	//bullet->setFilterGroup(this->getFilterGroup());
	bullet->setFilter(~(m_b2Body->GetFixtureList()->GetFilterData().maskBits), m_b2Body->GetFixtureList()->GetFilterData().maskBits); // UserData::Enemy << 1
	force *= 65000.0f;
	bullet->GetB2Body()->ApplyForceToCenter(force);

	gp.EnqueueAddToList(bullet);

}