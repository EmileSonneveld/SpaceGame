#pragma once

class entityBase;

class BallBase;
class Bullet;
class Enemy;
class Player;
class SpaceStation;
class SpriteAnimation;
class SpawnPoint;
//class UserData;

class BgElement;

#include <SFML/Graphics.hpp>

//#include "Sltn.h"
#include "entityBase.h"
#include "UserData.h"
//#include <Box2D/Box2D.h>
#include <Box2D/Common/b2Math.h>
#include <Box2D/Dynamics/b2Body.h>

#include "GameDefines.h"

#include <iostream>
#include <vector>
#include <forward_list>
#include <list>



class Gameplay
{

public:


	void Tick(const float deltaTime);
	void Paint(sf::RenderWindow& window);

	void LoadInktscapeFile(const char* HitregionsFile);

	b2Vec2 GetPlayerPos();

	void StickBodyToWorld(b2Body* body);

	void zoom(float delta){
		m_View.setSize(m_View.getSize()*delta);
	}
	void MakeCircle(sf::Vector2f place, float len, float distance );

	void ConnectWithOthers(BallBase* ballA);
	void EnqueueRemoveFromList(entityBase* ptr){
		m_entitiesRemoveFrom.push_back(ptr);
	}
	void EnqueueAddToList(BallBase* ptr){
		m_BallsToAdd.push_back(ptr);
		this->ConnectWithOthers(ptr);
	}
	void EnqueueAddToList(entityBase* ptr){
		m_entitiesToAdd.push_back(ptr);
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

	std::vector<sf::Drawable*> m_Drawables;

	sf::Sprite backgroundSpr;
	Player* m_player;
	std::vector<entityBase*> m_entities;
	std::vector<entityBase*> m_entitiesToAdd;
	std::vector<entityBase*> m_entitiesRemoveFrom;
	std::vector<BallBase*> m_Balls;
	std::vector<BallBase*> m_BallsToAdd;
	std::forward_list<Bullet*> m_bulletVec;
	std::vector<SpriteAnimation*> m_SpriteAnimationList;

	std::vector<SpawnPoint*> m_SpawnPointVec;

	void ApplyAddToQueue();
	void ApplyRemoveFrom();

	void RemoveFrom(entityBase* entity);
	// std::vector<Enemy*> m_EnemyVec;





	sf::View m_View;
	double m_timer;




	// Singleton stuff:
public:
	~Gameplay(void);

	static Gameplay& getInst(); // get the singleton reference

private:
	Gameplay(void); //:m_player(sf::Vector2f(50,50)){}
	static Gameplay*    instance;
};

