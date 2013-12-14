#include "sltn.h"
#include <Box2D\Box2D.h>
#include <iostream>

//void DrawShape(b2Fixture* fixture, const b2Transform& xf, const b2Color& color);

sltn::sltn():m_world( new b2World(b2Vec2(0,0)) )   {
	//m_world = new b2World(b2Vec2(0,0));
	//auto dbg= m_world->DrawDebugData();

}

const sf::Texture& sltn::GetTexture(const std::string& path){
	map<string,sf::Texture>::const_iterator it = m_SpriteMap.find(path); //map<string,sf::Texture>::const_iterator
	if( it==m_SpriteMap.end() ) {
		//sf::Texture tex;
		//tex.loadFromFile(path);
		//tex.setSmooth(true);
		//m_SpriteMap.insert( std::pair<std::string,sf::Texture>(path, tex)); // tex word meerdere keren gecopierd, daarom kan
		cout << "Loading Texture: " << path << "\n";
		m_SpriteMap.insert( std::pair<std::string,sf::Texture>(path, sf::Texture()));
		if( !m_SpriteMap[path].loadFromFile(path) ) 
			cout << " IMAGE NOT FOUND ";
		m_SpriteMap[path].setSmooth(true);
		m_SpriteMap[path].setRepeated(true);
		// m_SpriteMap.insert( std::pair<std::string,sf::Texture>(path, sf::Texture()) , it);
	}
	return m_SpriteMap[path];
}

void sltn::EnqueDestroyBody(b2Body* body){

	if( body == nullptr ) return;
	//if( !m_world->IsLocked() )
	//    m_world->DestroyBody(body);
	//else 
	bool found= false;
	for( b2Body* bodyIt= m_world->GetBodyList() ; bodyIt; bodyIt = bodyIt->GetNext()){
		//for (b2Body* body= sltn::getInst().m_world->GetBodyList(); body; body = body->GetNext()){
		if(bodyIt == body )
			found= true;
	}
	if( !found || body->GetContactList()== (b2ContactEdge*)0xfdfdfdfdfdfdfdfd)
		int ha=5;

	if( find(m_BodysToDelete.begin(), m_BodysToDelete.end(), body) !=  m_BodysToDelete.end() )
		return;

	m_BodysHaveBeenDeleted.push_back(body);
	m_BodysToDelete.push_back(body);

	auto ud= (UserData*)body->GetUserData();
	b2Assert(ud);
	( ud )->creator->nullB2Body();
}

void sltn::EnqueDestroyBody(b2Joint* body){

	if( body == nullptr ) return;
	//if( !m_world->IsLocked() )
	//    m_world->DestroyBody(body);
	//else 
	bool found= false;
	for( auto* bodyIt= m_world->GetJointList() ; bodyIt; bodyIt = bodyIt->GetNext()){
		//for (b2Body* body= sltn::getInst().m_world->GetBodyList(); body; body = body->GetNext()){
		if(bodyIt == body )
			found= true;
	}
	if( !found )
		return;

	//if( find(m_BodysToDelete.begin(), m_BodysToDelete.end(), body) !=  m_BodysToDelete.end() )
	//	return;

	//m_BodysHaveBeenDeleted.push_back(body);
	m_JointsToDelete.push_back(body);

	auto ud= (UserData*)body->GetUserData();
	// b2Assert(ud);
	// ( ud )->creator->nullB2Body();
}
void sltn::ExcecuteDestroyBodys(){
	assert(!m_world->IsLocked());
	if( m_world->IsLocked() )
		int kak=5;
	for( auto ptr : m_JointsToDelete){
		// delete ( (UserData*)ptr->GetUserData() );
		//ptr->SetUserData((void*)0xDE1E7ED);
		m_world->DestroyJoint(ptr);
	}
	m_JointsToDelete.clear();

	for( auto body : m_BodysToDelete){
		//auto contactList= body->GetContactList();
		delete ( (UserData*)body->GetUserData() );
		body->SetUserData((void*)0xDE1E7ED);
		m_world->DestroyBody(body);
		//memcpy(body, "Are you trying to acces something?",sizeof(*body));
	}
	m_BodysToDelete.clear();


}



b2Vec2 to_b2Vec2(sf::Vector2<float>& vec){
	return b2Vec2(vec.x, vec.y);
}

b2Vec2 to_b2Vec2(sf::Vector2<int>& vec){
	return b2Vec2((float)vec.x, (float)vec.y);
}

b2Vec2 to_b2Vec2(sf::Vector2<unsigned int>& vec){
	return b2Vec2((float)vec.x, (float)vec.y);
}

b2Vec2 to_b2Vec2(unsigned int x, unsigned int y){
	return b2Vec2((float)x, (float)y);
}


sf::Vector2<float> to_Vector2(b2Vec2 vec){
	return sf::Vector2<float>(vec.x, vec.y);
}
