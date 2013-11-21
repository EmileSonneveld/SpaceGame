#pragma once

#include <SFML/Graphics.hpp>
#include <Box2D\Box2D.h>
#include <string>
#include <map>
#include <typeinfo>

#include "entityBase.h"

using namespace std;
class b2World;
//class MainClass;

// Singleton
class sltn // final 
{
public:

    sf::Vector2<unsigned int> m_ScreenSize; // evil
    b2World* m_world;


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
    void RemoveBody(b2Body* body){
        m_BodysToDelete.push_back(body);
        if( !m_world->IsLocked() )
            m_world->DestroyBody(body);
    }
private:
    sltn() ;                   // Constructor? (the {} brackets) are needed here.
    sf::Vector2f m_mousePos;

    vector<b2Body*> m_BodysToDelete;
    map<std::string,sf::Texture> m_SpriteMap;

    sltn(sltn const&);              // Don't Implement
    void operator=(sltn const&); // Don't implement
};


b2Vec2 to_b2Vec2(sf::Vector2<float>& vec);
b2Vec2 to_b2Vec2(sf::Vector2<int>& vec);
sf::Vector2<float> to_Vector2(b2Vec2 vec);

struct UserData
{
    bool isConectedToCluster;
    //unsigned int clusterNumber;
    entityBase* creator;

    UserData(): isConectedToCluster(false)
    {
        //typeid(isConectedToCluster).raw_name();
    }
};