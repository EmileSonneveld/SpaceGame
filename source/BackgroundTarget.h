#pragma once
#include <SFML/Graphics.hpp>

class BackgroundTarget : public sf::Sprite
{
public:
	BackgroundTarget(float sizeX, float sizeY);
	~BackgroundTarget();
	// virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	sf::RenderTexture& GetTarget()
	{
		return m_RenderTexture;
	}

private:
	sf::RenderTexture m_RenderTexture;
	sf::View m_renderViewport;
};

