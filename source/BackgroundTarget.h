#pragma once
#include <SFML/Graphics.hpp>

class BackgroundTarget : public sf::Sprite
{
public:
	BackgroundTarget(float sizeX, float sizeY, sf::Vector2f pos );
	~BackgroundTarget();
	// virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	sf::RenderTexture& GetTarget()
	{
		return m_RenderTexture;
	}
	// Size in pixels of the texture
	static const unsigned int targetPixlSize = 1024 * 2;

private:
	sf::RenderTexture m_RenderTexture;
	sf::View m_renderViewport;
};

