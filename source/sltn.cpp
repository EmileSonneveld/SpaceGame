#include "sltn.h"
#include <Box2D\Box2D.h>
#include <iostream>

//void DrawShape(b2Fixture* fixture, const b2Transform& xf, const b2Color& color);

sltn::sltn()   {
    b2Vec2 gravity;
    gravity.Set(0.0f, 0.0f);
    m_world = new b2World(gravity);
    //auto dbg= m_world->DrawDebugData();



}
const sf::Texture& sltn::GetTexture(const std::string& path){
        map<string,sf::Texture>::const_iterator it = m_SpriteMapp.find(path); //map<string,sf::Texture>::const_iterator
        if( it==m_SpriteMapp.end() ) {
            //sf::Texture tex;
            //tex.loadFromFile(path);
            //tex.setSmooth(true);
            //m_SpriteMapp.insert( std::pair<std::string,sf::Texture>(path, tex)); // tex word meerdere keren gecopierd, daarom kan
            cout << "Loading Texture: " << path << "\n";
            m_SpriteMapp.insert( std::pair<std::string,sf::Texture>(path, sf::Texture()));
            if( !m_SpriteMapp[path].loadFromFile(path) ) 
                cout << " IMAGE NOT FOUND ";
            m_SpriteMapp[path].setSmooth(true);
           // m_SpriteMapp.insert( std::pair<std::string,sf::Texture>(path, sf::Texture()) , it);
        }
        return m_SpriteMapp[path];
    }


b2Vec2 to_b2Vec2(sf::Vector2<float>& vec){
    return b2Vec2(vec.x, vec.y);
}

b2Vec2 to_b2Vec2(sf::Vector2<int>& vec){
    return b2Vec2((float)vec.x, (float)vec.y);
}

sf::Vector2<float> to_Vector2(b2Vec2 vec){
    return sf::Vector2<float>(vec.x, vec.y);
}