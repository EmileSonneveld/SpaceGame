#pragma once

#include <SFML/Graphics.hpp>
#include <Box2D/Common/b2Settings.h>
class b2Body;

// Just to have a virtual destructor and sf::Sprite
// So the object can be passed as user data
class entityBase : public sf::Sprite
{
public:
	virtual void Tick(float dt) = 0;

	entityBase() : sf::Sprite(), m_b2Body(nullptr){}
	 
		virtual ~entityBase(){}
	 

		b2Body* GetB2Body(){ return m_b2Body; }
	 
		void nullB2Body(){ m_b2Body = nullptr; }
	 
		// use a negative number for no colition in the group
		void setFilterGroup(int16 group);
	int16 getFilterGroup();

protected:
	b2Body* m_b2Body;

};
