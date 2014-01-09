#pragma once

class b2Body;
//struct b2Vec2;
#include <Box2D/Common/b2Math.h>

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <vector>

// resambles a lot the sf::CircleShape

class BgElement : public sf::Shape
{
public:
	BgElement();
	virtual ~BgElement();

	std::vector<sf::Vertex> m_vertices;

	void SetAsOval(const b2Vec2 pos, const b2Vec2 radius);
	b2Body* m_Body;

	unsigned int m_pointCount;
	b2Vec2 m_radius;

	//virtual void 	draw(sf::RenderTarget &target, sf::RenderStates states) const = 0;

protected:
	virtual unsigned int getPointCount() const;
	virtual sf::Vector2f getPoint(unsigned int index) const;

};

