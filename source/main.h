#include <SFML/Graphics.hpp>
#include <iostream>
//#include "Bolleke.h"
#include "Ball.h"
#include "Bullet.h"
#include "Player.h"
#include "sltn.h"
#include <Box2D\Box2D.h>
#include <vector>

//class Player;
//class Ball;
//class Bullet;

int main(int argc, const char* argv[]);

class MainClass {

public:
    MainClass(); // mag maar 1 keer gemaakt worden
    void gameLoop(); // Call only one time!

    void LoopBalls(void func(Ball&));

private:

    void Tick(float deltaTime);
    void Paint(sf::RenderWindow& window);

    bool TryConnect();

    sf::Sprite backgroundSpr;
    Player m_player;
    std::vector<Ball> bollekesVec;
    std::vector<Bullet> m_bulletVec;

    sf::RenderWindow  m_window;

    void MainClass::DebugDraw();
    void MainClass::DrawShape(b2Fixture* fixture, const b2Transform& xf, const b2Color& color);

    sf::View m_View;

    //const MainClass* m_MainClass;

    MainClass(const MainClass&);
    MainClass& operator =(const MainClass&);
};

