#pragma once

#include <SFML/Graphics.hpp>
#include <Box2D/Common/b2Settings.h>
#include <Box2D/Dynamics/b2Fixture.h>
#include "Sltn.h"
class b2Body;
class b2World;

// Just to have a virtual destructor and sf::Sprite
// So the object can be passed as user data
class entityBase : public sf::Sprite
{
public:
	virtual void Tick(float dt) = 0;

	entityBase();

	virtual ~entityBase(){
		if (m_b2Body){
			Sltn::getInst().EnqueDestroyPhysicsEntity(m_b2Body);
			m_b2Body = nullptr;
		}
	}

	virtual void Initialize() = 0;

#ifdef _DEBUG
	static const unsigned int magicSize = 10;
	char memoryMagic[magicSize]; // view object in memory
#endif

	b2Body* GetB2Body(){ return m_b2Body; }

	void nullB2Body(){ m_b2Body = nullptr; }

	// use a negative number for no colition in the group
	void setFilterGroup(int16 group);
	void setFilter(int16 categoryBits, int16 maskBits);
	void setFilter(b2Filter filter);
	int16 getFilterGroup();

protected:
	b2Body* m_b2Body;

};
