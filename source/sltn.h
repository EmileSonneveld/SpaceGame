#pragma once

#include <SFML/Graphics.hpp>
#include <Box2D\Box2D.h>
#include <string>
#include <map>
#include <typeinfo>

#include "entityBase.h"

#include "../fmod_inc/fmod.hpp"
#include "../fmod_inc/common.h"

using namespace std;
class b2World;

// Singleton
class sltn // final 
{
public:

	sf::Vector2u m_ScreenSize; // evil
	b2World * const m_world;


public:
	// sltn::getInst().
	static sltn& getInst() // get the singleton reference
	{
		static sltn    instance; // Guaranteed to be destroyed.
		// Instantiated on first use.
		return instance;
	}
	 
		~sltn()
	{
			ExcecuteDestroyBodys();
			delete m_world;

			for (auto thing : m_BodysToDelete) {}

			FMOD_RESULT      result;
			for (auto soundPtr : m_Sounds){
				result = soundPtr->release();
				ERRCHECK(result);
			}

			result = system->close();
			ERRCHECK(result);
			result = system->release();
			ERRCHECK(result);

			Common_Close();
			 
		}
	 

		const sf::Texture& GetTexture(const std::string& path);

	// in World cordinates
	const sf::Vector2f& GetMousePos(){ return m_mousePos; }
	 
		void SetMousePos(const sf::Vector2f& pos){ m_mousePos = pos; }
	 
		void EnqueDestroyBody(b2Body* body);
	void EnqueDestroyBody(b2Joint* body);
	void ExcecuteDestroyBodys();




	FMOD::System    *system;
	vector<FMOD::Sound*> m_Sounds;
	FMOD::Sound* getSound(const char* str);
	void playSound(FMOD::Sound* soundPtr);

	void FmodStartup();

	int fmod_mainFunction();
	//FMOD::Sound     *sound1, *sound2, *sound3;
	FMOD::Channel   *channel1, *channel2, *channel3;
	FMOD_VECTOR      listenerpos;

private:
	sltn();
	sf::Vector2f m_mousePos;

	vector<b2Body*> m_BodysHaveBeenDeleted;
	vector<b2Body*> m_BodysToDelete;
	vector<b2Joint*> m_JointsToDelete;
	map<std::string, sf::Texture> m_SpriteMap;

	sltn(sltn const&);              // Don't Implement
	void operator=(sltn const&); // Don't implement
};


b2Vec2 to_b2Vec2(sf::Vector2<float>& vec);
b2Vec2 to_b2Vec2(sf::Vector2<int>& vec);
b2Vec2 to_b2Vec2(sf::Vector2<unsigned int>& vec);
b2Vec2 to_b2Vec2(unsigned int x, unsigned int y);
sf::Vector2<float> to_Vector2(b2Vec2 vec);

struct UserData
{
	enum Kind {
		unspecified = 0,
		player = -100,
		bullet = -101,
		ball = -102,
		Enemy = -103,
		SpriteAnimation = -104,
	};
	static const unsigned int magicSize = 8;
	char memoryMagic[magicSize]; // view object in memory
	Kind kind;
	bool isConectedToCluster;
	//unsigned int clusterNumber;
	entityBase *const creator;
	unsigned int cluster;

	UserData(entityBase* cr, Kind knd = UserData::unspecified) : creator(cr), isConectedToCluster(false), kind(knd)
	{
		//typeid(isConectedToCluster).raw_name();
#ifdef _DEBUG

		memcpy(memoryMagic, "UserData", magicSize);
#endif
	}
	 
};
