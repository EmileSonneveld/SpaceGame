#pragma once

class b2Body;
class b2Joint;
class b2World;

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/texture.hpp>

#include <Box2D\Common\b2Math.h>
#include <string>
#include <map>
#include <typeinfo>

#include "../fmod_inc/fmod.hpp"
#include "../fmod_inc/common.h"

using namespace std;

b2Vec2 to_b2Vec2(const sf::Vector2f& vec);
b2Vec2 to_b2Vec2(const sf::Vector2<int>& vec);
b2Vec2 to_b2Vec2(const sf::Vector2<unsigned int>& vec);
b2Vec2 to_b2Vec2(const unsigned int x, const unsigned int y);
sf::Vector2<float> to_Vector2(const b2Vec2 vec);

std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems);

const float FMOD_DistanceFactor = 1.0f;          // Units per meter.  I.e feet would = 3.28.  centimeters would = 100.

// Singleton
class sltn // final 
{
public:

	sf::Vector2u m_ScreenSize; // evil
	b2World * const m_world;

private:
	static sltn*    instance;
public:
	// sltn::getInst().
	static sltn& getInst() // get the singleton reference
	{
		//static sltn    instance; // Guaranteed to be destroyed.
		// Instantiated on first use.
		if (instance == nullptr) instance = new sltn();
		return *instance;
	}

	~sltn();


	const sf::Texture& GetTexture(const std::string& path);

	// in World cordinates
	const sf::Vector2f& GetMousePos(){ return m_mousePos; }

	void SetMousePos(const sf::Vector2f& pos){ m_mousePos = pos; }

	void EnqueDestroyPhysicsEntity(b2Body* body);
	void EnqueDestroyPhysicsEntity(b2Joint* body);
	void ExcecuteDestroyPhysicsEntities();




	FMOD::System    *system;
	std::map<std::string, FMOD::Sound*> m_Sounds;
	FMOD::Sound* getSound(const char* ptr);
	void playSound(FMOD::Sound* soundPtr);
	void playSound(FMOD::Sound* soundPtr,
		FMOD_VECTOR pos ,
		FMOD_VECTOR vel 
	);
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

