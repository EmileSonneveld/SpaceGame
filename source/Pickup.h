#pragma once

class b2Body;
//struct b2Vec2;
#include <Box2D/Common/b2Math.h>

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <vector>

// resambles a lot the sf::CircleShape

class Pickup : public sf::Drawable , sf::Transformable
{
public:
	Pickup(b2Vec2 pos);
	virtual ~Pickup();
	

	b2Body* m_b2Body;


	virtual void 	draw(sf::RenderTarget &target, sf::RenderStates states) const;

private:
	const sf::Texture* m_Texture;
	std::vector<sf::Vertex> m_vertices;

};

