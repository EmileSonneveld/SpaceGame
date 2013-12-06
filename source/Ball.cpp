#include "Ball.h"
#include "Player.h"
#include "sltn.h"
#include <Box2D\Box2D.h>

int numImages= 1; // 19
float rectWidth= 3097.0f/numImages;

Ball::Ball(sf::Vector2f pos) : entityBase() , m_radius(2.0f), m_lives(100.0)
{
	b2BodyDef bd;
	bd.type = b2_dynamicBody;
	bd.position.Set(pos.x, pos.y);
	bd.awake=false;
	auto ud= new UserData();
	ud->creator= this;
	ud->kind= UserData::ball;
	bd.userData= ud;
	m_b2Body = sltn::getInst().m_world->CreateBody(&bd);

	b2CircleShape shape;
	shape.m_radius= m_radius/2;
	//b2PolygonShape shape;
	//shape.SetAsBox(m_radius/2,m_radius/2);
	m_b2Body->CreateFixture(&shape, 1.0f);

	setTexture(sltn::getInst().GetTexture("resources/blue-sphere_512.png")); //Exp_yellow_maya blue-sphere_512
}

Ball::~Ball()
{
	if( m_b2Body )
		sltn::getInst().EnqueDestroyBody(m_b2Body);
	m_b2Body=nullptr;
}


void Ball::Tick(float dt) // 0.0166
{
	if( !m_b2Body ) return;

	auto prevRect = getTextureRect();
	prevRect.left+= rectWidth* (int)(  dt*rectWidth/2 );
	Sprite::setTextureRect(prevRect);

	auto pos= m_b2Body->GetPosition();
	setPosition(pos.x, pos.y);
	float angle= 180.0f/3.1415f*m_b2Body->GetTransform().q.GetAngle();
	Sprite::setRotation(angle);

}

void Ball::setTexture(const sf::Texture& texture, bool resetRect )
{
	Sprite::setTexture(texture, resetRect);

	rectWidth= (float)getTexture()->getSize().x/numImages;

	float scale= m_radius/getTexture()->getSize().y;
	Sprite::setOrigin( (float)texture.getSize().x/numImages/2.0f, (float)texture.getSize().y/2.0f );
	Sprite::setScale(scale,scale);
	Sprite::setTextureRect(sf::IntRect(0,0,rectWidth,getTexture()->getSize().y));
	//sf::Transformable::setOrigin(getTexture()->getSize()/2u); // todo: proper center
}