#pragma once

class entityBase;
class Ball;
class Player;
class Enemy;
class Bullet;
class SpaceStation;
class SpriteAnimation;

#include <SFML/Graphics.hpp>
#include <iostream>

#include "Player.h"
#include "sltn.h"
#include "entityBase.h"
#include <Box2D\Box2D.h>

#include <vector>
#include <forward_list>
#include <list>

void AddThickLine(sf::VertexArray& vertices, const sf::Vector2f& point1, const sf::Vector2f& point2);
bool AreLinqued(b2Body* bodyA, b2Body* bodyB);
bool CCW(b2Vec2 p1, b2Vec2 p2, b2Vec2 p3);
void ConnectTry(b2Body* bodyA, b2Body* bodyB); // Gameplay safties
bool ConnectBodys(b2Body* bodyA, b2Body* bodyB); // Just Do it!
unsigned int CountJoints(b2Body* body);
bool isIntersect(b2Vec2 p1, b2Vec2 p2, b2Vec2 q3, b2Vec2 q4);

class Gameplay
{

public:
	// Direct delete
	void Remove(entityBase* entity);

	void Tick(const float deltaTime);
	void Paint(sf::RenderWindow& window);

	

	b2Vec2 GetPlayerPos(){
		return m_player.GetB2Body()->GetPosition();
	}


	void zoom(float delta){
		m_View.setSize(m_View.getSize()*delta);
	}
	void MakeCircle(sf::Vector2f place, float len, float distance = Ball::semiGlobal_minDistance){
		float pi = 3.14159265f;
		float contour = 2.0f *len* pi;
		for (float radial = 0; radial < 2.0f * pi; radial += 2.0f*pi / (contour / distance)){

			auto pos = sf::Vector2f(len*cos(radial), len*sin(radial));
			pos += place;
			m_Balls.push_back(new Ball(pos));

			for (auto ball1 : m_Balls){
				// connect the last ball with the rest
				ConnectTry(ball1->GetB2Body(), m_Balls.back()->GetB2Body());
			}
		}
	}

	void ConnectWithOthers(Ball* ballA);

	void EnqueueAddToList(entityBase* ptr){
		m_entitiesToAdd.push_back(ptr);
	}
	void EnqueueAddToList(Ball* ptr){
		m_BallsToAdd.push_back(ptr);
	}
	void AddToList(SpriteAnimation* ptr){
		m_SpriteAnimationList.push_back(ptr);
	}

private:


	bool TryConnect();

	sf::Sprite backgroundSpr;
	Player m_player;
	std::vector<entityBase*> m_entities;
	std::vector<entityBase*> m_entitiesToAdd;
	std::vector<Ball*> m_Balls;
	std::vector<Ball*> m_BallsToAdd;
	std::forward_list<Bullet*> m_bulletVec;

	void ApplyAddToQueue(){
		for (auto ptr : m_entitiesToAdd)
			m_entities.push_back(ptr);
		m_entitiesToAdd.clear();

		for (auto ptr : m_BallsToAdd)
			m_Balls.push_back(ptr);
		m_BallsToAdd.clear();
	}

	// std::vector<Enemy*> m_EnemyVec;



	std::vector<SpriteAnimation*> m_SpriteAnimationList;


	sf::View m_View;
	double m_mouseTimer;




	// Singleton stuff:
public:
	~Gameplay(void);

	static Gameplay& getInst() // get the singleton reference
	{
		static Gameplay    instance; // Guaranteed to be destroyed.
		// Instantiated on first use.
		return instance;
	}

private:
	Gameplay(void); //:m_player(sf::Vector2f(50,50)){}


};

