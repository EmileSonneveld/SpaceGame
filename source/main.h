#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>


int main(int argc, const char* argv[]);

class MainClass {

public:
	MainClass(); // mag maar 1 keer gemaakt worden
	void gameLoop(); // Call only one time!

private:

	sf::RenderWindow  m_window;

	//const MainClass* m_MainClass;

	MainClass(const MainClass&);
	MainClass& operator =(const MainClass&);
}
 ;

