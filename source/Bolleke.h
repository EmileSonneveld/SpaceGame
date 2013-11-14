#pragma once
#include <SFML/Graphics.hpp>

class Bolleke : public sf::Drawable, public sf::Transformable
{
public:
	Bolleke(const sf::Texture& tex);
	virtual ~Bolleke(void);

	void Tick(double dt);

private:
	// word gacaled door  sf::window.draw(object);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const; 
	
	// transform cordinaten worden overgerft

	double m_radius;
	sf::Sprite m_sprite;
};

