#include "Player.h"
#include "sltn.h"
#include <Box2D\Box2D.h>
#include "main.h"


Player::Player(sf::Vector2f pos) : 
    entityBase(), m_radius(4.0f)
{
    b2BodyDef bd;
    bd.type = b2_dynamicBody;
    bd.position.Set(pos.x, pos.y);
    auto ud= new UserData();
    ud->creator= this;
    ud->kind= UserData::player;
    bd.userData= ud;
    m_b2Body = sltn::getInst().m_world->CreateBody(&bd);
    //m_vertices[0].texCoords=sf::Vector2f(-99,-99);
    b2CircleShape shape;
    //b2PolygonShape shape;
    //shape.SetAsBox(m_radius,m_radius);
    shape.m_radius= m_radius/2;
    //shape.SetAsBox(0.125f, 0.125f);
    m_b2Body->CreateFixture(&shape, 2.0f);
    //m_b2Body->ApplyForceToCenter(b2Vec2(rand()%100, -10));
    //setOrigin(m_radius/2,m_radius/2);
    //setOrigin(m_radius,m_radius);
}


Player::~Player()
{
    sltn::getInst().EnqueDestroyBody(m_b2Body);
    m_b2Body=nullptr;
}

void Player::Tick(float dt) // 0.0166
{
    if( !m_b2Body ) return;
    //m_b2Body->SetAngularVelocity(-m_b2Body->GetAngle());
    float angle= 180.0f/b2_pi*m_b2Body->GetTransform().q.GetAngle();
    Sprite::setRotation(angle);

    auto moveVector= b2Vec2(0,0); //sf::Vector2f();
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) moveVector.x-=1;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) moveVector.x+=1;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) moveVector.y-=1;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) moveVector.y+=1;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left )) moveVector.x-=1;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) moveVector.x+=1;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up   )) moveVector.y-=1;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down )) moveVector.y+=1;
    if(abs(moveVector.x)>0.01f || abs(moveVector.y)>0.01f){
        moveVector.Normalize();
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
            moveVector*=5.0f;
        moveVector*=800.0f;

        m_b2Body->ApplyForceToCenter(moveVector);
        //this->m_b2Body->ApplyLinearImpulse(to_b2Vec2(moveVector*60.0f), b2Vec2(0,0));
    }

    if( m_b2Body ){
        auto pos= m_b2Body->GetPosition();
        Sprite::setPosition(pos.x, pos.y);
        //setPosition(pos.x-getOrigin().x, pos.y-getOrigin().y);
        //float angle= 180.0f/3.1415f*m_b2Body->GetTransform().q.GetAngle();
        //Sprite::setRotation(angle);
        //setRotation(angle);
    }

}

//void Player::draw(RenderTarget& target, RenderStates states) const{}

void Player::setTexture(const sf::Texture& texture, bool resetRect )
{
    sf::Sprite::setTexture(texture, resetRect);
    //sf::Sprite::setColor(sf::Color(100, 60, 100));
    float scale= m_radius/getTexture()->getSize().x;
    setOrigin((float)texture.getSize().x/2,(float)texture.getSize().y/2);
    setScale(scale*2,scale*2); // special image
    //sf::Transformable::setOrigin(getTexture()->getSize()/2u); // todo: proper center
}