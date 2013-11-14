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
    const sf::Texture& GetTexture(const std::string& path){
        map<string,sf::Texture>::const_iterator it = m_SpriteMapp.find(path); //map<string,sf::Texture>::const_iterator
        if( it==m_SpriteMapp.end() ) {
            //sf::Texture tex;
            //tex.loadFromFile(path);
            //tex.setSmooth(true);
            //m_SpriteMapp.insert( std::pair<std::string,sf::Texture>(path, tex)); // tex word meerdere keren gecopierd, daarom kan
            m_SpriteMapp.insert( std::pair<std::string,sf::Texture>(path, sf::Texture()));
            m_SpriteMapp[path].loadFromFile(path);
            m_SpriteMapp[path].setSmooth(true);
           // m_SpriteMapp.insert( std::pair<std::string,sf::Texture>(path, sf::Texture()) , it);
        }else{
            int ha= 5; 
        }
        return m_SpriteMapp[path];
    }

   
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


//b2Vec2 to_b2Vec2(sf::Vector2f vec);
b2Vec2 to_b2Vec2(sf::Vector2<float>& vec);
b2Vec2 to_b2Vec2(sf::Vector2<int>& vec);

struct UserData
{
    bool isConectedToCluster;

    UserData(): isConectedToCluster(false)
    {

    }
};