#include <SFML/Graphics.hpp>
#include <iostream>
#include "Ball.h"
#include "Player.h"
#include "sltn.h"
#include <Box2D\Box2D.h>
#include <vector>
#include "main.h"
#include "Bullet.h"
#include "MyWorldCallbacks.hpp"
#include "Gameplay.h"

using namespace std;


int main(int argc, const char* argv[])
{
	// _crtBreakAlloc= 161;
	//_CrtSetBreakAlloc(1);


	//auto g_mainClass= new MainClass();
	auto g_mainClass = new MainClass();
	g_mainClass->gameLoop();

	// _CrtDumpMemoryLeaks();
}

MainClass::MainClass() :
m_window(sf::RenderWindow(sf::VideoMode(1280, 720), "SFML Space game By Emile"))
{
	m_window.setVerticalSyncEnabled(true);
	sltn::getInst().m_ScreenSize = m_window.getSize();

	sltn::getInst().m_world->SetContactListener(new MyContactListener());
	sltn::getInst().m_world->SetDestructionListener(new MyDestructionListener());
	m_window.setView(sf::View(sf::FloatRect()));


	{
		b2BodyDef bd;
		bd.type = b2_staticBody;
		bd.allowSleep = false;
		bd.userData = nullptr;
		bd.position.Set(0.0f, 250.0f);//(float)sltn::getInst().m_ScreenSize.y
		b2Body* body = sltn::getInst().m_world->CreateBody(&bd);

		b2PolygonShape shape;
		shape.SetAsBox(1100, 50); // (float)sltn::getInst().m_ScreenSize.x
		//shape.SetAsBox(0.125f, 0.125f);
		body->CreateFixture(&shape, 1.0f);
	}
}

void MainClass::gameLoop(){
	sf::Clock Clock;
	float time = 0.01f;
	while (m_window.isOpen())
	{
		sf::Event event;
		while (m_window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				m_window.close();

			else if (event.type == sf::Event::MouseWheelMoved)
			{
				auto zoomerke = (float)event.mouseWheel.delta;
				zoomerke = 1 - zoomerke / 4;
				Gameplay::getInst().zoom(zoomerke);
			}
			else if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::Escape)
				{
					m_window.close();
				}
			}
			else if (event.type == sf::Event::MouseButtonPressed)
			{
				sf::Mouse::getPosition() - m_window.getPosition();

			}
			//if (event.type == sf::Event::Resized)
			//{
			//    m_window.setSize(sf::Vector2u(event.size.width,event.size.height));
			//}
		}


		time += (Clock.getElapsedTime().asSeconds() - time) / 10.0f;
		time = min(0.25f, time); // geen physics explosion plz 4FPS min
		Clock.restart();

		sltn::getInst().SetMousePos(
			m_window.mapPixelToCoords(sf::Mouse::getPosition(m_window))
			);

		m_window.clear(sf::Color(40, 40, 80));
		Gameplay::getInst().Tick(time);
		Gameplay::getInst().Paint(m_window);



		sltn::getInst().ExcecuteDestroyBodys();
		sltn::getInst().m_world->Step(time, 8, 3);

		m_window.display();
	}
}

/*
void MainClass::Tick(const float deltaTime){

}

void MainClass::Paint(sf::RenderWindow& window)
{

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

*/
