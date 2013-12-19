#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>

#include "Ball.h"
#include "Bullet.h"
#include "Player.h"
#include "SpriteAnimation.h"
#include "sltn.h"
#include "entityBase.h"
#include <Box2D\Box2D.h>

#include <vector>
#include <forward_list>
#include <list>

class Enemy;
class SpaceStation;

class Gameplay 
{

public:
	// Direct delete
	void Remove(entityBase* entity){
		for( auto& ptr : m_bollekesVec){
			if( ptr == entity ) {
				delete entity;
				ptr= nullptr;
				return;
			}
		}
		for( auto& ptr : m_SpriteAnimationList){
			if( ptr == entity ) {
				delete entity;
				ptr= nullptr;
				return;
			}
		}
		//for( auto& ptr : m_bulletVec)
		//    if( ptr == bal ) {
		//        ptr= nullptr;
		//        return;
		//    }
		//sltn::getInst().RemoveBody(entity.)
	}

	void Tick(const float deltaTime);
	void Paint(sf::RenderWindow& window);

	void Add(SpriteAnimation* ptr){
		m_SpriteAnimationList.push_back(ptr);
	}

	b2Vec2 GetPlayerPos(){
		return m_player.GetB2Body()->GetPosition();
	}

	void zoom(float delta){
		m_View.setSize( m_View.getSize()*delta );
	}
private:

	bool TryConnect();
	void Connect(b2Body* bodyA, b2Body* bodyB);

	sf::Sprite backgroundSpr;
	Player m_player;
	std::vector<Ball*> m_bollekesVec;
	std::forward_list<Bullet*> m_bulletVec;
	std::vector<entityBase*> m_entities;
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

