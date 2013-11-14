#include "Bolleke.h"


Bolleke::Bolleke(const sf::Texture& tex): Transformable(), Drawable(),
	m_radius(100)
{
	m_sprite.setTexture(tex);

	m_sprite.setScale(m_radius/tex.getSize().x,m_radius/tex.getSize().x);
	//m_sprite.setColor(sf::Color(255, 205, 205, 200)); // half transparent
	m_sprite.setPosition(400,200 );
	m_sprite.rotate(1);
}


Bolleke::~Bolleke(void)
{
}

void Bolleke::Tick(double dt) // 0.0166
{
	m_sprite.move(10,300 );
	//m_sprite.setPosition( sf::Transformable::getPosition() );
}

void Bolleke::draw(sf::RenderTarget& target, sf::RenderStates states) const
{

	target.draw(m_sprite);

}
