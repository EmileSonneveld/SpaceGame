#include "SpaceStation.h"
#include "Player.h"
#include "Sltn.h"
#include "Gameplay.h"
#include <Box2D\Box2D.h>
#include <SFML/Graphics.hpp>

SpaceStation::SpaceStation(sf::Vector2f pos) : entityBase(), m_va(sf::VertexArray(sf::PrimitiveType::Quads))
{
	b2BodyDef bd;
	bd.type = b2_dynamicBody;// b2_kinematicBody;
	bd.position.Set(pos.x, pos.y);
	if( !bd.position.IsValid() ) return;
	bd.awake=false;
	auto ud= new UserData(this);
	//ud->creator= this;
	ud->kind= UserData::unspecified;
	bd.userData= ud;
	m_b2Body = Sltn::getInst().m_world->CreateBody(&bd);

	m_b2Body->SetAngularVelocity(0.5);
	originalPos= m_b2Body->GetPosition();

	b2PolygonShape shape[4];
	float size= 20;
	shape[0].SetAsBox(size,2, b2Vec2(-size*2.5f,0), 0);	  m_b2Body->CreateFixture(shape+0, 2.0f);
	shape[1].SetAsBox(size,2, b2Vec2(+size*2.5f,0), 0);	  m_b2Body->CreateFixture(shape+1, 2.0f);
	shape[2].SetAsBox(2,size, b2Vec2(0,-size*2.5f), 0);	  m_b2Body->CreateFixture(shape+2, 2.0f);
	shape[3].SetAsBox(2,size, b2Vec2(0,+size*2.5f), 0);	  m_b2Body->CreateFixture(shape+3, 2.0f);

	setTexture(Sltn::getInst().GetTexture("resources/foto.jpg")); //Exp_yellow_maya blue-sphere_512


	for (auto fixture = m_b2Body->GetFixtureList(); fixture; fixture = fixture->GetNext()){
		auto shape = (b2PolygonShape*)fixture->GetShape();
		auto vec = (shape)->m_vertices;
		for (int i = 0; i<shape->GetVertexCount(); ++i){
			//auto vert= transform.getInverse().transformPoint(sf::Vector2f( vec[i].x,vec[i].y));
			auto vert = sf::Vector2f(vec[i].x, vec[i].y);
			m_va.append(sf::Vertex(vert, sf::Color(50, 80, 90, 200), vert));
		}
	}

}

SpaceStation::~SpaceStation()
{
	if( m_b2Body ) Sltn::getInst().EnqueDestroyPhysicsEntity(m_b2Body);
	m_b2Body=nullptr;
}


void SpaceStation::Tick(float dt) // 0.0166
{

	if( m_b2Body ) // Sync body to sprite
	{
		//m_b2Body->SetAngularVelocity(5);
		//m_b2Body->ApplyForce(b2Vec2(50,50),b2Vec2(0,0));

		auto force= originalPos - m_b2Body->GetPosition();
		force*= 1000.0f;
		m_b2Body->ApplyForceToCenter(force,true);

		auto pos= m_b2Body->GetPosition();
		Sprite::setPosition(pos.x, pos.y);

		float angle= 180.0f/b2_pi*m_b2Body->GetTransform().q.GetAngle();
		Sprite::setRotation(angle);
	}
}


void SpaceStation::setTexture(const sf::Texture& texture, int nrOfFrames )
{
	Sprite::setTexture(texture, false);

	auto rectWidth= (float)getTexture()->getSize().x;
	float size=9;
	//Sprite::setOrigin( (float)size/2.0f, (float)size/2.0f );
	//Sprite::setScale(size/getTexture()->getSize().x,size/getTexture()->getSize().y);
	Sprite::setTextureRect(sf::IntRect(0,0, (int)rectWidth, getTexture()->getSize().y) );
}

void SpaceStation::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	//states.transform= Vie //sf::Transform::Identity;//this->getTransform();
	states.texture = nullptr; //getTexture();

	//Sprite::draw(target, states);
	//target.draw(*this, states);


	

	target.draw(m_va, states);


}