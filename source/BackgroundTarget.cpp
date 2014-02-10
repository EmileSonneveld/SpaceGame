#include "BackgroundTarget.h"


BackgroundTarget::BackgroundTarget(float sizeX, float sizeY, sf::Vector2f pos)
: m_RenderTexture()
, m_renderViewport(sf::FloatRect(pos.x, pos.y, sizeX, sizeY))
{
	m_RenderTexture.create(targetPixlSize, targetPixlSize);
	m_RenderTexture.setView(m_renderViewport);
	m_RenderTexture.clear(sf::Color(0, 50, 0, 0));

	setTexture(m_RenderTexture.getTexture());
	m_RenderTexture.display();


	sf::Transformable::setScale(m_renderViewport.getSize() / static_cast<float>(targetPixlSize));
	sf::Transformable::setRotation(m_renderViewport.getRotation());
	sf::Transformable::setPosition(m_renderViewport.getCenter());
	sf::Transformable::setOrigin(m_RenderTexture.getSize().x / 2U, m_RenderTexture.getSize().y / 2U);
}


BackgroundTarget::~BackgroundTarget()
{
}

/*
void BackgroundTarget::draw(sf::RenderTarget& target, sf::RenderStates states) const
{

}*/