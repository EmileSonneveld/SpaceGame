#include <SFML/Graphics.hpp>
#include <iostream>
#include <Box2D/Box2D.h>
#include <vector>
#include "main.h"
#include "MyWorldCallbacks.hpp"
#include "DebugDraw.h"
#include "Gameplay.h"
#include "Sltn.h"

#include <signal.h>
#define _CRT_SECURE_NO_WARNINGS
// #include <lua.hpp>
//
// static int report(lua_State *L, int status) {
// 	if (status != LUA_OK && !lua_isnil(L, -1)) {
// 		const char *msg = lua_tostring(L, -1);
// 		if (msg == NULL) msg = "(error object is not a string)";
// 		cerr << ("lua", msg);
// 		lua_pop(L, 1);
// 		/* force a complete garbage collection in case of errors */
// 		lua_gc(L, LUA_GCCOLLECT, 0);
// 	}
// 	return status;
// }
//
// /* the next function is called unprotected, so it must avoid errors */
// static void finalreport(lua_State *L, int status) {
// 	if (status != LUA_OK) {
// 		const char *msg = (lua_type(L, -1) == LUA_TSTRING) ? lua_tostring(L, -1)
// 			: NULL;
// 		if (msg == NULL) msg = "(error object is not a string)";
// 		cerr << ("lua", msg);
// 		lua_pop(L, 1);
// 	}
// }
//
// static int traceback(lua_State *L) {
// 	const char *msg = lua_tostring(L, 1);
// 	if (msg)
// 		luaL_traceback(L, L, msg, 1);
// 	else if (!lua_isnoneornil(L, 1)) {  /* is there an error object? */
// 		if (!luaL_callmeta(L, 1, "__tostring"))  /* try its 'tostring' metamethod */
// 			lua_pushliteral(L, "(no error message)");
// 	}
// 	return 1;
// }
//
// static int docall(lua_State *L, int narg, int nres) {
// 	int status;
// 	int base = lua_gettop(L) - narg;  /* function index */
// 	lua_pushcfunction(L, traceback);  /* push traceback function */
// 	lua_insert(L, base);  /* put it under chunk and args */
// 	//globalL = L;  /* to be available to 'laction' */
// 	//signal(SIGINT, laction);
// 	status = lua_pcall(L, narg, nres, base);
// 	signal(SIGINT, SIG_DFL);
// 	lua_remove(L, base);  /* remove traceback function */
// 	return status;
// }
//
// static int dofile(lua_State *L, const char *name) {
// 	int status = luaL_loadfile(L, name);
// 	if (status == LUA_OK) status = docall(L, 0, 0);
// 	return report(L, status);
// }
//
// static int l_sin(lua_State *L) {
// 	double d = lua_tonumber(L, 1);  /* get argument */
// 	lua_pushnumber(L, sin(d));  /* push result */
// 	return 1;  /* number of results */
// }


int main(int argc, const char* argv[])
{

#ifdef DETECT_MEMLEAKS
	// _crtBreakAlloc= 161;
	_CrtSetBreakAlloc(1);
#endif




	//auto g_mainClass= new MainClass();
	auto mainClass = new MainClass();
	mainClass->gameLoop();


#ifdef DETECT_MEMLEAKS
	// Delete singletons to exit grasefully
	delete &Gameplay::getInst();
	delete &Sltn::getInst();
	delete mainClass;
	_CrtDumpMemoryLeaks();
#endif

}

MainClass::MainClass() :
m_window(sf::VideoMode(1280, 720), "SFML Space game By Emile")
{
	m_window.setVerticalSyncEnabled(true);
	Sltn::getInst().m_ScreenSize = m_window.getSize();
	// this 'new'-stuff needs to be freed at the end
	Sltn::getInst().m_world->SetContactListener(new MyContactListener());
	Sltn::getInst().m_world->SetDestructionListener(new MyDestructionListener());
	auto dbgDraw = new MyDebugDraw();
	dbgDraw->SetRenderWindow(&m_window);
	Sltn::getInst().m_world->SetDebugDraw(dbgDraw);
	m_window.setView(sf::View(sf::FloatRect()));


}

int MainClass::gameLoop(){


	//  int status, result;
	//  lua_State *L = luaL_newstate();  /* create state */
	//  if (L == NULL) {
	//  	cerr << ("Lua: cannot create state: not enough memory");
	//  	return EXIT_FAILURE;
	//  }
	//  /* call 'pmain' in protected mode */
	//  //lua_pushcfunction(L, &pmain);
	//  dofile(L, "resources/stuff.lua");
	//  //lua_pushinteger(L, argc);  /* 1st argument */
	//  //lua_pushlightuserdata(L, argv); /* 2nd argument */
	//  status = lua_pcall(L, 2, 1, 0);
	//  result = lua_toboolean(L, -1);  /* get result */



	sf::Clock Clock;
	float deltaTime = 0.01f;
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
				zoomerke = 1 - zoomerke / 7;
				Gameplay::getInst().zoom(zoomerke);
			}
			else if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::Escape)
				{
					m_window.close();
				}
			}
			//else if (event.type == sf::Event::MouseButtonPressed)
			//{
			//	sf::Mouse::getPosition() - m_window.getPosition();
			//}
			//if (event.type == sf::Event::Resized)
			//{
			//    m_window.setSize(sf::Vector2u(event.size.width,event.size.height));
			//}
		}


		// Smootherd dt
		deltaTime += (Clock.getElapsedTime().asSeconds() - deltaTime) / 10.0f;
		deltaTime = min(0.25f, deltaTime); // geen physics explosion plz 4FPS min
		Clock.restart();


		m_window.clear(sf::Color(40, 40, 80));

		// Lua Wip
		// lua_getglobal(L, "UpdateMyStuff");
		// lua_pushnumber(L, deltaTime);
		// lua_call(L, 1, 0);

		Gameplay::getInst().Tick(deltaTime);
		Gameplay::getInst().Paint(m_window);
		Sltn::getInst().m_world->DrawDebugData();

		Sltn::getInst().SetMousePos(
			m_window.mapPixelToCoords(sf::Mouse::getPosition(m_window))
			);


		Sltn::getInst().ExcecuteDestroyPhysicsEntities();
		Sltn::getInst().m_world->Step(0.01f, 8, 3); //deltaTime

		m_window.display();
	}


	//finalreport(L, status);
	//lua_close(L);
	//return (result && status == LUA_OK) ? EXIT_SUCCESS : EXIT_FAILURE;
	return 0;
}

/*
void MainClass::Tick(const float deltaTime){

}

void MainClass::Paint(sf::RenderWindow& window)
{

}


void MainClass::DebugDraw(){
auto drawer= this; //Sltn::getInst().m_world;
for (b2Body* b = Sltn::getInst().m_world->GetBodyList(); b; b = b->GetNext())
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
