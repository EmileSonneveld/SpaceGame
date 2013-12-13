#include "Ball.h"
#include "Player.h"
#include "sltn.h"
#include <Box2D\Box2D.h>

//__declspec(dllimport) b2Body * __cdecl b2World::CreateBody(b2BodyDef const *);
//#pragma import sfml-graphics-d-2.lib
 // #pragma comment(lib, "sfml-graphics-d-2.lib")

Ball::Ball(sf::Vector2f pos, float radius) : entityBase() , m_radius(radius), m_lives(100.0f)
{
	b2BodyDef bd;
	bd.type = b2_dynamicBody;
	bd.position.Set(pos.x, pos.y);
	if( !bd.position.IsValid() ) return;
	bd.awake=false;
	auto ud= new UserData();
	ud->creator= this;
	ud->kind= UserData::unspecified;
	bd.userData= ud;
	m_b2Body = sltn::getInst().m_world->CreateBody(&bd);

	b2CircleShape shape;	shape.m_radius= m_radius/2;
	//b2PolygonShape shape;   shape.SetAsBox(m_radius/2,m_radius/2);
	m_b2Body->CreateFixture(&shape, 1.0f);

	setTexture(sltn::getInst().GetTexture("resources/blue-sphere_512.png")); //Exp_yellow_maya blue-sphere_512
}

Ball::~Ball()
{
	if( m_b2Body ) sltn::getInst().EnqueDestroyBody(m_b2Body);
	m_b2Body=nullptr;
}


void Ball::Tick(float dt) // 0.0166
{
	CustomStuff(dt);


	if( m_b2Body ) // Sync body to sprite
	{
		auto pos= m_b2Body->GetPosition();
		Sprite::setPosition(pos.x, pos.y);

		float angle= 180.0f/b2_pi*m_b2Body->GetTransform().q.GetAngle();
		Sprite::setRotation(angle);
	}
}


void Ball::setTexture(const sf::Texture& texture, int nrOfFrames )
{
	Sprite::setTexture(texture, false);

	auto rectWidth= (float)getTexture()->getSize().x/nrOfFrames;

	float scale= m_radius/getTexture()->getSize().y;
	Sprite::setOrigin( (float)texture.getSize().x/nrOfFrames/2.0f, (float)texture.getSize().y/2.0f );
	Sprite::setScale(scale,scale);
	Sprite::setTextureRect(sf::IntRect(0,0, (int)rectWidth, getTexture()->getSize().y) );
}