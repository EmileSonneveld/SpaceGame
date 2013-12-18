#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>

#include "Ball.h"
#include "Bullet.h"
#include "Player.h"
#include "sltn.h"
#include "entityBase.h"
#include <Box2D\Box2D.h>

#include <vector>
#include <forward_list>
#include <list>

//class Player;
//class Ball;
//class Bullet;


int main(int argc, const char* argv[]);

class MainClass {

public:
    MainClass(); // mag maar 1 keer gemaakt worden
    void gameLoop(); // Call only one time!

private:

    sf::RenderWindow  m_window;

    // void MainClass::DebugDraw();
    // void MainClass::DrawShape(b2Fixture* fixture, const b2Transform& xf, const b2Color& color);


    //const MainClass* m_MainClass;

    MainClass(const MainClass&);
    MainClass& operator =(const MainClass&);
};



// Global MAIN class! evil!
//extern MainClass* g_mainClass;
