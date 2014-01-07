#pragma once

class entityBase;

class Ball;
class Bullet;
class Enemy;
class Player;
class SpaceStation;
class SpriteAnimation;
//class UserData;

#include <SFML/Graphics.hpp>

//#include "sltn.h"
#include "entityBase.h"
#include "UserData.h"
//#include <Box2D/Box2D.h>
#include <Box2D/Common/b2Math.h>
#include <Box2D/Dynamics/b2Body.h>

#include <iostream>
#include <vector>
#include <forward_list>
#include <list>

void AddThickLine(sf::VertexArray& vertices, const sf::Vector2f& point1, const sf::Vector2f& point2);
bool AreLinqued(b2Body* bodyA, b2Body* bodyB);
bool CCW(b2Vec2 p1, b2Vec2 p2, b2Vec2 p3);
bool ConnectTry(b2Body* bodyA, b2Body* bodyB); // Gameplay safties
bool ConnectBodys(b2Body* bodyA, b2Body* bodyB); // Just Do it!
unsigned int CountJoints(b2Body* body);
bool isIntersect(b2Vec2 p1, b2Vec2 p2, b2Vec2 q3, b2Vec2 q4);

class Gameplay
{

public:


	void Tick(const float deltaTime);
	void Paint(sf::RenderWindow& window);



	b2Vec2 GetPlayerPos();


	void zoom(float delta){
		m_View.setSize(m_View.getSize()*delta);
	}
	void MakeCircle(sf::Vector2f place, float len, float distance );

	void ConnectWithOthers(Ball* ballA);
	void EnqueueRemoveFromList(entityBase* ptr){
		m_entitiesRemoveFrom.push_back(ptr);
	}
	void EnqueueAddToList(entityBase* ptr){
		m_entitiesToAdd.push_back(ptr);
	}
	void EnqueueAddToList(Ball* ptr){
		m_BallsToAdd.push_back(ptr);
	}
	void AddToList(SpriteAnimation* ptr){
		m_SpriteAnimationList.push_back(ptr);
	}

	int16 GetGroupForKind(UserData::Kind kind){

	}
	void AddKill(){ ++m_NrOfKills; }

private:
	int m_NrOfKills;
	void PaintGui(sf::RenderWindow& window);
	sf::Font m_Font;

	bool TryConnect();

	sf::Sprite backgroundSpr;
	Player* m_player;
	std::vector<entityBase*> m_entities;
	std::vector<entityBase*> m_entitiesToAdd;
	std::vector<entityBase*> m_entitiesRemoveFrom;
	std::vector<Ball*> m_Balls;
	std::vector<Ball*> m_BallsToAdd;
	std::forward_list<Bullet*> m_bulletVec;
	std::vector<SpriteAnimation*> m_SpriteAnimationList;

	void ApplyAddToQueue(){
		for (auto ptr : m_entitiesToAdd)
			m_entities.push_back(ptr);
		m_entitiesToAdd.clear();

		for (auto ptr : m_BallsToAdd){
			m_Balls.push_back(ptr);
			this->ConnectWithOthers(ptr);
		}
		m_BallsToAdd.clear();
	}
	void ApplyRemoveFrom(){
		for (auto ptr : m_entitiesRemoveFrom){
			RemoveFrom(ptr);
		}
		m_entitiesRemoveFrom.clear();
	}

	void RemoveFrom(entityBase* entity);
	// std::vector<Enemy*> m_EnemyVec;





	sf::View m_View;
	double m_mouseTimer;




	// Singleton stuff:
public:
	~Gameplay(void);

	static Gameplay& getInst(); // get the singleton reference

private:
	Gameplay(void); //:m_player(sf::Vector2f(50,50)){}
	static Gameplay*    instance;
};

