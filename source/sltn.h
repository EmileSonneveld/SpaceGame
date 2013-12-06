#pragma once

#include <SFML/Graphics.hpp>
#include <Box2D\Box2D.h>
#include <string>
#include <map>
#include <typeinfo>

#include "entityBase.h"

using namespace std;
class b2World;

// Singleton
class sltn // final 
{
public:

    sf::Vector2u m_ScreenSize; // evil
    b2World * const m_world;


public:
    // sltn::getInst().
    static sltn& getInst() // get the singleton reference
    {
        static sltn    instance; // Guaranteed to be destroyed.
        // Instantiated on first use.
        return instance;
    }

    const sf::Texture& GetTexture(const std::string& path);

    // in World cordinates
    const sf::Vector2f& GetMousePos(){return m_mousePos;}
    void SetMousePos(const sf::Vector2f& pos){m_mousePos= pos;}
    void EnqueDestroyBody(b2Body* body);
    void ExcecuteDestroyBodys();
private:
    sltn() ; 
    sf::Vector2f m_mousePos;

    vector<b2Body*> m_BodysHaveBeenDeleted;
    vector<b2Body*> m_BodysToDelete;
    map<std::string,sf::Texture> m_SpriteMap;

    sltn(sltn const&);              // Don't Implement
    void operator=(sltn const&); // Don't implement
};


b2Vec2 to_b2Vec2(sf::Vector2<float>& vec);
b2Vec2 to_b2Vec2(sf::Vector2<int>& vec);
b2Vec2 to_b2Vec2(sf::Vector2<unsigned int>& vec);
b2Vec2 to_b2Vec2(unsigned int x, unsigned int y);
sf::Vector2<float> to_Vector2(b2Vec2 vec);

struct UserData
{
    enum kind {
        unspecified,
        player= 0x12121212,
        bullet= 0x34343434,
        ball  = 0x56565656,
		SpriteAnimation,
    };
    static const unsigned int magicSize=8;
    char memoryMagic[magicSize]; // view object in memory
    kind kind;
    bool isConectedToCluster;
    //unsigned int clusterNumber;
    entityBase* creator;

    UserData(): isConectedToCluster(false), kind(UserData::unspecified)
    {
        //typeid(isConectedToCluster).raw_name();
        memcpy(memoryMagic, "UserData", magicSize);
    }
};