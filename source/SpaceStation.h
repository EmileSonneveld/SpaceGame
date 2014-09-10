#pragma once

#include "entityBase.h"

class b2Body;
struct b2Vec2;
class RenderTarget;

#include <Box2D/Box2D.h>

//#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics.hpp>

class SpaceStation : public entityBase
{
public:
	SpaceStation(sf::Vector2f pos);
	virtual ~SpaceStation();
	virtual void Initialize(){};

	void Tick(float dt);

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

protected:
	b2Vec2 originalPos;
	void setTexture(const sf::Texture& texture, int nrOfFrames=1); // bevat extras

	sf::VertexArray m_va;
private:

};

