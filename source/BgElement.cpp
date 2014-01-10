#include "BgElement.h"
#include <Box2D/Box2D.h>
#include "UserData.h"
#include "Sltn.h"
//#include "GameDefines.h"
using namespace sf;


BgElement::BgElement() : m_vertices(), m_b2Body(nullptr), m_pointCount(0), m_radius(1,1)
{

}


BgElement::~BgElement()
{
	Sltn::getInst().EnqueDestroyPhysicsEntity(m_b2Body);
}

void BgElement::SetAsOval(const b2Vec2 pos, const b2Vec2 radius)
{
	m_radius = radius;
	m_pointCount = 10 + (unsigned int)m_radius.x / 4;
	this->setPosition(pos.x - radius.x, pos.y - radius.y);


	b2BodyDef bodyDef;
	bodyDef.type = b2_staticBody;

	bodyDef.position = pos;

	if (!bodyDef.position.IsValid()) return;
	bodyDef.awake = false;
	auto userData = new UserData(nullptr);
	//userData->creator = this;
	userData->kind = UserData::Static;
	bodyDef.userData = userData;
	auto m_b2Body = Sltn::getInst().m_world->CreateBody(&bodyDef);

	b2CircleShape shape;
	shape.m_radius = min(radius.x, radius.y);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &shape;
	//fixtureDef.friction = 0;
	fixtureDef.density = 1;
	//fixtureDef.restitution = 0.1;
	//fixtureDef.filter.groupIndex = 0;
	m_b2Body->CreateFixture(&fixtureDef);

	update();
}

// void 	BgElement::draw(RenderTarget &target, RenderStates states) const{}


////////////////////////////////////////////////////////////
unsigned int BgElement::getPointCount() const
{
	return m_pointCount;
}


////////////////////////////////////////////////////////////
Vector2f BgElement::getPoint(unsigned int index) const
{
	static const float pi = 3.141592654f;

	float angle = index * 2 * pi / m_pointCount - pi / 2;
	float x = std::cos(angle) * m_radius.x;
	float y = std::sin(angle) * m_radius.y;

	return Vector2f(m_radius.x + x, m_radius.y + y);
}