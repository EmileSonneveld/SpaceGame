#pragma once
#include <SFML/Graphics.hpp>

class b2Body;
class MainClass;

class Player : public sf::Sprite
{
public:
	Player(sf::Vector2f pos, const MainClass* ptr);
	virtual ~Player();

	void Tick(float dt);
	//virtual void draw(RenderTarget& target, RenderStates states) const;

	void setTexture(const sf::Texture& texture, bool resetRect = false); // bevat extras
	b2Body* GetB2Body(){return m_b2Body;}

private:
	const MainClass* m_MainClassPtr;

	b2Body* m_b2Body;
	float m_radius;
};

