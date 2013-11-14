#include "sltn.h"
#include <Box2D\Box2D.h>

//void DrawShape(b2Fixture* fixture, const b2Transform& xf, const b2Color& color);

sltn::sltn()   {
    b2Vec2 gravity;
    gravity.Set(0.0f, 0.0f);
    m_world = new b2World(gravity);
    //auto dbg= m_world->DrawDebugData();



}
/*
sf::Texture& sltn::GetSprite(const std::string& path){
    auto it = m_SpriteMap.find(path); //map<string,sf::Texture>::const_iterator
    if( it!=m_SpriteMap.end() ) {
        sf::Texture tex;
        m_SpriteMap[path].loadFromFile(string("resources/")+path);
        m_SpriteMap.insert( path,tex );
    }
    return m_SpriteMap[path];
}
*/

b2Vec2 to_b2Vec2(sf::Vector2<float>& vec){
    return b2Vec2(vec.x, vec.y);
}

b2Vec2 to_b2Vec2(sf::Vector2<int>& vec){
    return b2Vec2((float)vec.x, (float)vec.y);
}