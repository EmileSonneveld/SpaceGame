#include "BackgroundTarget.h"


BackgroundTarget::BackgroundTarget(float sizeX = 100.0f, float sizeY = 100.0f)
: m_RenderTexture()
, m_renderViewport(sf::FloatRect(0, 0, sizeX, sizeY))
{
	m_RenderTexture.create(1024, 1024);
	m_RenderTexture.setView(m_renderViewport);
	m_RenderTexture.clear(sf::Color(0, 50, 0, 128));

	setTexture(m_RenderTexture.getTexture());
	m_RenderTexture.display();


	sf::Transformable::setScale(m_renderViewport.getSize() / 1024.0f);
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