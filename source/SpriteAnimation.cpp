#include "SpriteAnimation.h"
#include "Player.h"
#include "Gameplay.h"
#include "sltn.h"
#include <Box2D\Box2D.h>



SpriteAnimation::SpriteAnimation(sf::Vector2f pos, bool looping) : entityBase() , 
	m_looping(looping), 
	m_radius(4.0f),
	m_numImages(19),
	m_rectWidth(3097.0f/m_numImages) // tmp invulling
{
	setPosition(pos);

	b2BodyDef bd;
	bd.type = b2_staticBody;
	bd.position.Set(pos.x, pos.y);
	bd.awake=false;
	auto ud= new UserData();
	ud->creator= this;
	ud->kind= UserData::SpriteAnimation;
	bd.userData= ud;
	m_b2Body = sltn::getInst().m_world->CreateBody(&bd);
	//m_b2Body->SetTransform(to_b2Vec2(pos), 0); // b2Vec2()
	// b2CircleShape shape;
	// shape.m_radius= m_radius/2;
	// //b2PolygonShape shape;
	// //shape.SetAsBox(m_radius/2,m_radius/2);
	// m_b2Body->CreateFixture(&shape, 1.0f);


	setTexture(sltn::getInst().GetTexture("resources/Exp_yellow_maya.png")); //Exp_yellow_maya blue-sphere_512
}

SpriteAnimation::~SpriteAnimation()
{
	if( m_b2Body )
		sltn::getInst().EnqueDestroyBody(m_b2Body);
	m_b2Body=nullptr;
}


void SpriteAnimation::Tick(float dt) // 0.0166
{
	auto prevRect = getTextureRect();
	prevRect.left+= m_rectWidth* (int)(  dt*m_rectWidth/2 );
	Sprite::setTextureRect(prevRect);

	if( !m_b2Body ) return;

	auto pos= m_b2Body->GetPosition();
	setPosition(pos.x, pos.y);
	float angle= 180.0f/3.1415f*m_b2Body->GetTransform().q.GetAngle();
	Sprite::setRotation(angle);

	if(prevRect.left >= getTexture()->getSize().x ) Gameplay::getInst().Remove(this);
}

void SpriteAnimation::setTexture(const sf::Texture& texture, bool resetRect )
{
	Sprite::setTexture(texture, resetRect);

	m_rectWidth= (float)getTexture()->getSize().x/m_numImages;

	float scale= m_radius/getTexture()->getSize().y;
	Sprite::setOrigin( (float)m_rectWidth/2.0f, (float)texture.getSize().y/2.0f );
	Sprite::setScale(scale,scale);
	Sprite::setTextureRect(sf::IntRect(0,0,(int)m_rectWidth, getTexture()->getSize().y));
	//sf::Transformable::setOrigin(getTexture()->getSize()/2u); // todo: proper center
}