#include "Pickup.h"
#include <Box2D/Box2D.h>
#include "UserData.h"
#include "Sltn.h"
//#include "GameDefines.h"
using namespace sf;


Pickup::Pickup(b2Vec2 pos) : m_vertices(),
m_b2Body(nullptr),
m_Texture(&Sltn::getInst().GetTexture("resources/pickup1.gif"))  //SpaceCore.jpg")  
{
	Transformable::setPosition(pos.x, pos.y);
	float size = 2;

	b2BodyDef bodyDef;
	bodyDef.type = b2_staticBody;

	bodyDef.position = pos;

	if (!bodyDef.position.IsValid()) return;
	bodyDef.awake = false;
	auto userData = new UserData(nullptr);
	//userData->creator = this;
	userData->kind = UserData::Pickup;
	bodyDef.userData = userData;
	auto m_b2Body = Sltn::getInst().m_world->CreateBody(&bodyDef);

	b2CircleShape shape;
	shape.m_radius = size;

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &shape;
	//fixtureDef.friction = 0;
	fixtureDef.density = 1;
	//fixtureDef.restitution = 0.1;
	//fixtureDef.filter.groupIndex = 0;
	m_b2Body->CreateFixture(&fixtureDef);

	// update();


	m_vertices.push_back(Vertex(Vector2f(-size, -size), Vector2f(0, 0)));
	m_vertices.push_back(Vertex(Vector2f(+size, -size), Vector2f(m_Texture->getSize().x, 0)));
	m_vertices.push_back(Vertex(Vector2f(+size, +size), Vector2f(m_Texture->getSize().x, m_Texture->getSize().y)));
	m_vertices.push_back(Vertex(Vector2f(-size, +size), Vector2f(0, m_Texture->getSize().y)));

}


Pickup::~Pickup()
{
	Sltn::getInst().EnqueDestroyPhysicsEntity(m_b2Body);
}


void Pickup::draw(RenderTarget &target, RenderStates states) const
{
	states.transform *= getTransform();
	states.texture = m_Texture;
	//states.shader = &sf::Shader();
	target.draw(m_vertices.data(), m_vertices.size(), sf::PrimitiveType::Quads, states);
}

