#pragma once

#include <SFML/Graphics.hpp>
#include <Box2D\Box2D.h>
#include <string>
#include <map>
using namespace std;
class b2World;
//class MainClass;

// Singleton
class sltn
{
public:
    //virtual ~sltn(void);


    sf::Vector2<unsigned int> m_ScreenSize; // evil
    b2World* m_world;


public:
    static sltn& getInst() // get the singleton reference
    {
        static sltn    instance; // Guaranteed to be destroyed.
        // Instantiated on first use.

        return instance;
    }
    const sf::Texture& GetTexture(const std::string& path);

   
private:
    sltn() ;                   // Constructor? (the {} brackets) are needed here.
    sf::Vector2i m_mousePos;

    map<std::string,sf::Texture> m_SpriteMapp;

    // Dont forget to declare these two. You want to make sure they
    // are unaccessable otherwise you may accidently get copies of
    // your singleton appearing.
    sltn(sltn const&);              // Don't Implement
    void operator=(sltn const&); // Don't implement
    //friend MainClass;
};


b2Vec2 to_b2Vec2(sf::Vector2<float>& vec);
b2Vec2 to_b2Vec2(sf::Vector2<int>& vec);
sf::Vector2<float> to_Vector2(b2Vec2 vec);

struct UserData
{
    bool isConectedToCluster;

    UserData(): isConectedToCluster(false)
    {

    }
};