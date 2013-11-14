#include <SFML/Graphics.hpp>
#include <iostream>
#include "Bolleke.h"
#include "Ball.h"
#include "Player.h"
#include "sltn.h"
#include <Box2D\Box2D.h>
#include <vector>
#include "main.h"
using namespace std;

 MainClass* g_mainClass= new MainClass();

int main(int argc, const char* argv[])
{
    g_mainClass->gameLoop();
    //MainClass();
}

MainClass::MainClass():
    m_window(sf::RenderWindow(sf::VideoMode(1280, 720), "SFML Space game"))
    ,player(Player(sf::Vector2f(50,50), this))
    ,m_View(sf::FloatRect(0, 0, 100, 60))
{
    m_window.setVerticalSyncEnabled(true);
    sltn::getInst().m_ScreenSize= m_window.getSize();

    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);


    m_window.setView(m_View);



    sf::Texture texture;
    if (!texture.loadFromFile("resources/space.jpg")) // in project folder
    {
        int ha=5;
    }
    texture.setSmooth(true);
    //texture.setRepeated(true); tiled
    backgroundSpr.setTexture(texture);
    backgroundSpr.setPosition(sf::Vector2f(0, 0));

    sf::Texture texBol;
    if (!texBol.loadFromFile("resources/blue-sphere_512.png")) // in project folder
    {
    }
    for (unsigned int i = 0; i < 1000; i++)
    {
        auto pos= sf::Vector2f((float)(rand()%sltn::getInst().m_ScreenSize.x/8) , (float)(rand()%sltn::getInst().m_ScreenSize.y));
        auto object= Ball(pos);
        object.setTexture(texBol);
        //object.setPosition();
        bollekesVec.push_back( object );
    }

    player.setTexture(texBol);


    {
        b2BodyDef bd;
        bd.type = b2_staticBody;
        bd.allowSleep = false;
        bd.position.Set(0.0f,(float)sltn::getInst().m_ScreenSize.y);
        b2Body* body = sltn::getInst().m_world->CreateBody(&bd);

        b2PolygonShape shape;
        shape.SetAsBox((float)sltn::getInst().m_ScreenSize.x,50);
        //shape.SetAsBox(0.125f, 0.125f);
        body->CreateFixture(&shape, 1.0f);
    }
    if( false)
    {
        b2Body* ground = sltn::getInst().m_world->CreateBody(&b2BodyDef());

        b2BodyDef bd;
        bd.type = b2_dynamicBody;
        bd.allowSleep = false;
        bd.position.Set(0.0f, 10.0f);
        b2Body* body = sltn::getInst().m_world->CreateBody(&bd);

        float size= 60.0f;
        b2PolygonShape shape;
        shape.SetAsBox(5.5f, size, b2Vec2( 10.0f, 0.0f), 0.0);
        body->CreateFixture(&shape, 5.0f);
        shape.SetAsBox(5.5f, size, b2Vec2(-10.0f, 0.0f), 0.0);
        body->CreateFixture(&shape, 5.0f);
        shape.SetAsBox(size, 0.5f, b2Vec2(0.0f, 10.0f), 0.0);
        body->CreateFixture(&shape, 5.0f);
        shape.SetAsBox(size, 0.5f, b2Vec2(0.0f, -10.0f), 0.0);
        body->CreateFixture(&shape, 5.0f);

        b2RevoluteJointDef jd;
        jd.bodyA = ground;
        jd.bodyB = body;
        jd.localAnchorA.Set(0.0f, 10.0f);
        jd.localAnchorB.Set(0.0f, 0.0f);
        jd.referenceAngle = 0.0f;
        jd.motorSpeed = 0.05f * b2_pi;
        jd.maxMotorTorque = 1e8f;
        jd.enableMotor = true;
        auto joint = (b2RevoluteJoint*)sltn::getInst().m_world->CreateJoint(&jd);
        //joint->set
    }
}

void MainClass::gameLoop(){
    sf::Clock Clock;
    while (m_window.isOpen())
    {
        sf::Event event;
        while (m_window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                m_window.close();

            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Escape)
                {
                    m_window.close();
                }
            }
            if (event.type == sf::Event::MouseButtonPressed)
            {
                sf::Mouse::getPosition() - m_window.getPosition() ;

            }
            if (event.type == sf::Event::Resized)
            {
                m_window.setSize(sf::Vector2u(event.size.width,event.size.height));
            }
        }


        float time = Clock.getElapsedTime().asSeconds();
        time= min(0.25f,time); // geen physics explosion plz 4FPS min
        Clock.restart();


        Tick(time);

        m_window.clear(sf::Color::Blue);
        Paint(m_window);



        sltn::getInst().m_world->Step(time,8,3);
        m_window.display();
    }
}

bool AreLinqued(b2Body* bodyA,b2Body* bodyB)
{
    if( bodyA== bodyB ) return true;
    for(b2JointEdge* j =bodyB->GetJointList();j;j=j->next ){
        if( j->other == bodyA ) 
            return true;
    }
    return false;
}

unsigned int CountJoints(b2Body* body){
    unsigned int returner=0;
    for(b2JointEdge* j =body->GetJointList();j;j=j->next )
        ++returner;
    return returner;
}

struct ChosenBody{
    ChosenBody(
        b2Body* bodyA,
        float distanceA
        //unsigned int numOfJointsA
        ):	bodyA(bodyA), distanceSquared(distanceA), numOfJoints(0)
    {
        numOfJoints= CountJoints(bodyA);
    }
    b2Body* bodyA;
    b2Body* bodyB;
    float distanceSquared;
    unsigned int numOfJoints; // get calculated
};

bool CCW(b2Vec2 p1, b2Vec2 p2, b2Vec2 p3) {
    float a = p1.y; float b = p1.x; 
    float c = p2.y; float d = p2.x;
    float e = p3.y; float f = p3.x;
    return (f - b) * (c - a) > (d - b) * (e - a);
}

bool isIntersect(b2Vec2 p1, b2Vec2 p2, b2Vec2 q3, b2Vec2 q4) {
    float side1= (p2.x - p1.x)*(q3.y - p2.y) < (p2.y - p1.y)*(q3.x - p2.x);
    float side2= (p2.x - p1.x)*(q4.y - p2.y) < (p2.y - p1.y)*(q4.x - p2.x);
    return (side1 == side2);
    // (Fx - Ex)(Py - Fy) - (Fy - Ey)(Px - Fx);
    // (Fx - Ex)(Qy - Fy) - (Fy - Ey)(Qx - Fx);
}

void ConnectBodys(b2Body* bodyA, b2Body* bodyB){
    auto diffVec= (bodyA->GetPosition()-bodyB->GetPosition());
    b2WeldJointDef jd; //b2RevoluteJointDef b2WeldJoint  b2DistanceJointDef
    jd.bodyA = bodyA;
    jd.bodyB = bodyB;
    //jd.localAnchorA.Set(0,0);//+sqrt(squared)/2);
    jd.localAnchorB.Set(diffVec.x, diffVec.y);//-sqrt(squared)/2);
    //jd.localAnchorA.Set(bodyB->GetPosition().x, bodyB->GetPosition().y);//+sqrt(squared)/2);
    //jd.localAnchorB.Set(bodyA->GetPosition().x, bodyA->GetPosition().y);//-sqrt(squared)/2);
    jd.frequencyHz = 20.0f;
    //jd.referenceAngle= 0; //atan2f(diffVec.x, diffVec.y);
    //jd.dampingRatio=1;
    //jd.collideConnected= false;
    //jd.length=5;
    //jd.referenceAngle = 0.0f;
    //jd.motorSpeed = 0.05f * b2_pi;
    //jd.maxMotorTorque = 1e8f;
    //jd.enableMotor = true;
    sltn::getInst().m_world->CreateJoint(&jd);

    ((UserData*)bodyB->GetUserData() )->isConectedToCluster = true;
}

void MainClass::TryConnect()
{
    for (auto& ball : bollekesVec){
        b2Body* bodyA= player.GetB2Body(); // player
        b2Body* bodyB= ball.GetB2Body(); // ball
        if( bodyB == nullptr ) return;
        auto diffVec= (bodyA->GetPosition()-bodyB->GetPosition());
        auto squared= diffVec.LengthSquared();

        //if( ((UserData*)bodyA->GetUserData() )->isConectedToCluster == true ) continue;
        if( ((UserData*)bodyB->GetUserData() )->isConectedToCluster == true ) 				continue;

        if ( AreLinqued(bodyA, bodyB) ) continue;
        if( CountJoints(bodyA) >10 ) continue;
        if(  squared>50) continue;

        ConnectBodys(bodyA,bodyB);
    }

    vector<ChosenBody> chosenBodys;
    for (auto& ballA : bollekesVec){
        b2Body* bodyA= ballA.GetB2Body(); 
        if( ((UserData*)bodyA->GetUserData() )->isConectedToCluster != true ) continue;

        for (auto& ballB : bollekesVec){

            b2Body* bodyB= ballB.GetB2Body(); 
            auto diffVec= (bodyA->GetPosition()-bodyB->GetPosition());
            auto squared= diffVec.LengthSquared();

            //if( ((UserData*)bodyB->GetUserData() )->isConectedToCluster == true ) continue;

            if ( AreLinqued(bodyB, bodyA) ) {
                int kak=5;
                continue;
            }
            if ( AreLinqued(bodyA, bodyB) ) {
                int kak=5;
                continue;
            }

            if( squared>50) continue;
            if( squared<20) continue;
            auto chosenBody = ChosenBody( bodyA, squared );
            chosenBody.bodyB= bodyB;
            if( chosenBody.numOfJoints >4 ) continue;

            chosenBodys.push_back(chosenBody);

        }
    }
    if( chosenBodys.size()<=0 ) return;

    // if( chosenBodys.size()<2 ) 
    ConnectBodys(chosenBodys[0].bodyA, chosenBodys[0].bodyB);
    for( auto chosenBody : chosenBodys )
    {

    }
}

void MainClass::Tick(float deltaTime){
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::X)){
        //if( player.GetB2Body()-> ==nullptr ) return;
        sltn::getInst().m_world->DestroyJoint(
            player.GetB2Body()->GetJointList()->joint);
        player.GetB2Body()->GetJointList()->joint = nullptr;
    }

    player.Tick(deltaTime);
    for (auto& object : bollekesVec)
        object.Tick(deltaTime);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
        TryConnect();

    m_View.setCenter(player.getPosition());
}

void MainClass::Paint(sf::RenderWindow& window)
{
    m_window.draw(backgroundSpr);

    m_window.setView(m_View);
    for (auto& object : bollekesVec)
        m_window.draw(object);

    m_window.draw(player);

    unsigned int bCount= sltn::getInst().m_world->GetBodyCount();
    sf::VertexArray vertexArray(sf::Lines, bCount);
    for (b2Body* body = sltn::getInst().m_world->GetBodyList(); body; body = body->GetNext()){
        for(b2JointEdge* j =body->GetJointList();j;j=j->next ){
            vertexArray.append(sf::Vertex(
                sf::Vector2f(j->other->GetPosition().x,j->other->GetPosition().y), sf::Color::Red ));
            vertexArray.append(sf::Vertex(
                sf::Vector2f(body->GetPosition().x,body->GetPosition().y), sf::Color::Red ));
        }
    }
    m_window.draw(vertexArray);
    //sf::Shape Line   = sf::Shape::Line(X1, Y1, X2, Y2, Thickness, Color, , [OutlineColor]);
    //m_window.draw(Line);

    // sltn::getInst().m_world->SetWarmStarting(settings->enableWarmStarting > 0);
    // sltn::getInst().m_world->SetContinuousPhysics(settings->enableContinuous > 0);
    // sltn::getInst().m_world->SetSubStepping(settings->enableSubStepping > 0);
    // sltn::getInst().m_world->Step(time, settings->velocityIterations, settings->positionIterations);

    //DebugDraw();
}


void MainClass::DebugDraw(){
    auto drawer= this; //sltn::getInst().m_world;
    for (b2Body* b = sltn::getInst().m_world->GetBodyList(); b; b = b->GetNext())
    {
        const b2Transform& xf = b->GetTransform();
        for (b2Fixture* f = b->GetFixtureList(); f; f = f->GetNext())
        {
            if      (b->IsActive() == false)			{ drawer->DrawShape(f, xf, b2Color(0.5f, 0.5f, 0.3f));	}
            else if (b->GetType() == b2_staticBody)		{ drawer->DrawShape(f, xf, b2Color(0.5f, 0.9f, 0.5f));	}
            else if (b->GetType() == b2_kinematicBody)	{ drawer->DrawShape(f, xf, b2Color(0.5f, 0.5f, 0.9f));	}
            else if (b->IsAwake() == false)				{ drawer->DrawShape(f, xf, b2Color(0.6f, 0.6f, 0.6f));	}
            else										{ drawer->DrawShape(f, xf, b2Color(0.9f, 0.7f, 0.7f));	}
        }
    }
}

void MainClass::DrawShape(b2Fixture* fixture, const b2Transform& xf, const b2Color& color)
{
    switch (fixture->GetType())
    {
    case b2Shape::e_circle:
        {
            b2CircleShape* circle = (b2CircleShape*)fixture->GetShape();

            b2Vec2 center = b2Mul(xf, circle->m_p);
            float32 radius = circle->m_radius;
            b2Vec2 axis = b2Mul(xf.q, b2Vec2(1.0f, 0.0f));

            auto shape=sf::CircleShape(radius,5);
            shape.setPosition(center.x,center.y);
            m_window.draw(shape);
            //m_debugDraw->DrawSolidCircle(center, radius, axis, color);
        }
        break;

    case b2Shape::e_edge:
        {
            b2EdgeShape* edge = (b2EdgeShape*)fixture->GetShape();
            b2Vec2 v1 = b2Mul(xf, edge->m_vertex1);
            b2Vec2 v2 = b2Mul(xf, edge->m_vertex2);
            //m_debugDraw->DrawSegment(v1, v2, color);
        }
        break;

    case b2Shape::e_chain:
        {
            b2ChainShape* chain = (b2ChainShape*)fixture->GetShape();
            int32 count = chain->m_count;
            const b2Vec2* vertices = chain->m_vertices;

            b2Vec2 v1 = b2Mul(xf, vertices[0]);
            for (int32 i = 1; i < count; ++i)
            {
                b2Vec2 v2 = b2Mul(xf, vertices[i]);
                //m_debugDraw->DrawSegment(v1, v2, color);
                //m_debugDraw->DrawCircle(v1, 0.05f, color);
                v1 = v2;
            }
        }
        break;

    case b2Shape::e_polygon:
        {
            b2PolygonShape* poly = (b2PolygonShape*)fixture->GetShape();
            int32 vertexCount = poly->m_vertexCount;
            b2Assert(vertexCount <= b2_maxPolygonVertices);
            b2Vec2 vertices[b2_maxPolygonVertices];

            for (int32 i = 0; i < vertexCount; ++i)
            {
                vertices[i] = b2Mul(xf, poly->m_vertices[i]);
            }

            //m_debugDraw->DrawSolidPolygon(vertices, vertexCount, color);
        }
        break;

    default:
        break;
    }
}