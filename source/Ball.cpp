#include "Ball.h"
#include "Player.h"
#include "sltn.h"
#include <Box2D\Box2D.h>



Ball::Ball(sf::Vector2f pos) : entityBase() , m_radius(2.0f), m_lives(100.0)
{
    b2BodyDef bd;
    bd.type = b2_dynamicBody;
    bd.position.Set(pos.x, pos.y);
    bd.awake=false;
    auto ud= new UserData();
    ud->creator= this;
    bd.userData= ud;
    m_b2Body = sltn::getInst().m_world->CreateBody(&bd);

    b2CircleShape shape;
    shape.m_radius= m_radius/2;
    //b2PolygonShape shape;
    //shape.SetAsBox(m_radius/2,m_radius/2);
    m_b2Body->CreateFixture(&shape, 1.0f);
    setTexture(sltn::getInst().GetTexture("resources/blue-sphere_512.png"));
}

Ball::~Ball()
{
    sltn::getInst().m_world->DestroyBody(m_b2Body);
}


void Ball::Tick(float dt) // 0.0166
{
    if( !m_b2Body ){

    } else {
        auto pos= m_b2Body->GetPosition();
        setPosition(pos.x, pos.y);
        float angle= 180.0f/3.1415f*m_b2Body->GetTransform().q.GetAngle();
        Sprite::setRotation(angle);
        // sltn::getInst().m_world->DestroyBody(m_b2Body);
        // m_b2Body=nullptr;
    }
    
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
    {

    }
}

void Ball::setTexture(const sf::Texture& texture, bool resetRect )
{
    sf::Sprite::setTexture(texture, resetRect);

    float scale= m_radius/getTexture()->getSize().x;
    setOrigin(texture.getSize().x/2U,texture.getSize().y/2U);
    setScale(scale,scale);
    //sf::Transformable::setOrigin(getTexture()->getSize()/2u); // todo: proper center
}