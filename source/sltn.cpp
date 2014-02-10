#include "Sltn.h"
#include "UserData.h"
#include "entityBase.h"
#include <Box2D\Box2D.h>
#include <iostream>
#include <sstream>

#include "GameDefines.h"
//void DrawShape(b2Fixture* fixture, const b2Transform& xf, const b2Color& color);
Sltn* Sltn::instance = nullptr;

Sltn::Sltn() :m_world(new b2World(b2Vec2(0, 0)))   {
	//m_world = new b2World(b2Vec2(0,0));
	//auto dbg= m_world->DrawDebugData();
	FmodStartup();

	/*{
		b2BodyDef bd;
		bd.type = b2_staticBody;
		bd.allowSleep = false;
		bd.userData = nullptr;
		bd.position.Set(0.0f, 250.0f);//(float)Sltn::getInst().m_ScreenSize.y
		b2Body* body = Sltn::getInst().m_world->CreateBody(&bd);

		b2PolygonShape shape;
		shape.SetAsBox(1100, 50); // (float)Sltn::getInst().m_ScreenSize.x
		//shape.SetAsBox(0.125f, 0.125f);
		body->CreateFixture(&shape, 1.0f);
		}*/

}
Sltn::~Sltn()
{
	ExcecuteDestroyPhysicsEntities();
	//auto body = m_world->GetBodyList();
	//auto bodyBefore = body;
	//while (body){
	//	//if (!body) break;
	//	m_world->DestroyBody(body);
	//	bodyBefore = body;
	//	body = body->GetNext();
	//}

	//for (b2Body* body = Sltn::getInst().m_world->GetBodyList(); body; body = body->GetNext()){
	//	m_world->DestroyBody(body);
	//}

	delete m_world;


	FMOD_RESULT      result;
	for (auto soundPtr : m_Sounds){
		result = soundPtr.second->release();
		ERRCHECK(result);
	}

	result = system->close();
	ERRCHECK(result);
	result = system->release();
	ERRCHECK(result);

	Common_Close();
	instance = nullptr;
}
const sf::Texture& Sltn::GetTexture(const std::string& path){
	map<string, sf::Texture>::const_iterator it = m_SpriteMap.find(path); //map<string,sf::Texture>::const_iterator
	if (it == m_SpriteMap.end()) {
		//sf::Texture tex;
		//tex.loadFromFile(path);
		//tex.setSmooth(true);
		//m_SpriteMap.insert( std::pair<std::string,sf::Texture>(path, tex)); // tex word meerdere keren gecopierd, daarom kan
		cout << "Loading Texture: " << path << "\n";
		m_SpriteMap.insert(std::pair<std::string, sf::Texture>(path, sf::Texture()));
		if (!m_SpriteMap[path].loadFromFile(path))
			cout << " IMAGE NOT FOUND ";
		m_SpriteMap[path].setSmooth(true);
		m_SpriteMap[path].setRepeated(true);
		// m_SpriteMap.insert( std::pair<std::string,sf::Texture>(path, sf::Texture()) , it);
	}
	return m_SpriteMap[path];
}

void Sltn::EnqueDestroyPhysicsEntity(b2Body* body){

	if (body == nullptr) return;
	//if( !m_world->IsLocked() )
	//    m_world->DestroyBody(body);
	//else 
	bool found = false;
	for (b2Body* bodyIt = m_world->GetBodyList(); bodyIt; bodyIt = bodyIt->GetNext()){
		//for (b2Body* body= Sltn::getInst().m_world->GetBodyList(); body; body = body->GetNext()){
		if (bodyIt == body)
			found = true;
	}
	assert(found);

	if (!found || body->GetContactList() == (b2ContactEdge*)0xfdfdfdfdfdfdfdfd)
		int ha = 5;

	if (find(m_BodysToDelete.begin(), m_BodysToDelete.end(), body) != m_BodysToDelete.end())
		return;

	m_BodysHaveBeenDeleted.push_back(body);
	m_BodysToDelete.push_back(body);

	auto ud = (UserData*)body->GetUserData();
	b2Assert(ud);
	if (ud->creator) ud->creator->nullB2Body();
}

void Sltn::EnqueDestroyPhysicsEntity(b2Joint* body){

	if (body == nullptr) return;
	//if( !m_world->IsLocked() )
	//    m_world->DestroyBody(body);
	//else 
	bool found = false;
	for (auto* bodyIt = m_world->GetJointList(); bodyIt; bodyIt = bodyIt->GetNext()){
		//for (b2Body* body= Sltn::getInst().m_world->GetBodyList(); body; body = body->GetNext()){
		if (bodyIt == body)
			found = true;
	}
	if (!found)
		return;

	//if( find(m_BodysToDelete.begin(), m_BodysToDelete.end(), body) !=  m_BodysToDelete.end() )
	//	return;

	//m_BodysHaveBeenDeleted.push_back(body);
	m_JointsToDelete.push_back(body);

	auto ud = (UserData*)body->GetUserData();
	// b2Assert(ud);
	// ( ud )->creator->nullB2Body();
}

void Sltn::ExcecuteDestroyPhysicsEntities(){
	assert(!m_world->IsLocked());
	if (m_world->IsLocked())
		int kak = 5;
	for (auto ptr : m_JointsToDelete){
		// delete ( (UserData*)ptr->GetUserData() );
		//ptr->SetUserData((void*)0xDE1E7ED);
		m_world->DestroyJoint(ptr);
	}
	m_JointsToDelete.clear();

	for (auto body : m_BodysToDelete){
		//auto contactList= body->GetContactList();
		delete ((UserData*)body->GetUserData());
		body->SetUserData((void*)0xDE1E7ED);
		m_world->DestroyBody(body);
		//memcpy(body, "Are you trying to acces something?",sizeof(*body));
	}
	m_BodysToDelete.clear();


}





const int   INTERFACE_UPDATETIME = 50;      // 50ms update for interface

void Sltn::FmodStartup()
{
	FMOD_RESULT      result;
	listenerpos.x = 0;
	listenerpos.y = 0;
	listenerpos.z = -1.0f * FMOD_DistanceFactor;
	// listenerpos  = { 0.0f, 0.0f, -1.0f * FMOD_DistanceFactor };
	unsigned int     version;
	void            *extradriverdata = 0;

	Common_Init(&extradriverdata);

	/*
	Create a System object and initialize.
	*/
	result = FMOD::System_Create(&system);
	ERRCHECK(result);

	result = system->getVersion(&version);
	ERRCHECK(result);

	if (version < FMOD_VERSION)
	{
		Common_Fatal("FMOD lib version %08x doesn't match header version %08x", version, FMOD_VERSION);
	}

	result = system->init(100, FMOD_INIT_NORMAL | FMOD_INIT_PROFILE_ENABLE, extradriverdata);
	ERRCHECK(result);
	/*
	Set the distance units. (meters/feet etc).
	*/

	result = system->set3DSettings(1.0, FMOD_DistanceFactor, 1.0f);
	ERRCHECK(result);

	fmod_mainFunction();
}

FMOD::Sound* Sltn::getSound(const char* ptr)
{
	string str(ptr);

	// Check if the sound is already loaded
	std::map<std::string, FMOD::Sound*>::const_iterator it = m_Sounds.find(str); //map<string,sf::Texture>::const_iterator
	if (it != m_Sounds.end()) return (*it).second;

	// We don't wan't' fmod to concern about errors
	if (!FileExists(str)) {
		cerr << "File sound not found: " << str << "\n";
		return nullptr;
	}

	// Realy load the stuff:
	FMOD_RESULT      result;
	FMOD::Sound* pSound;
	result = system->createSound(Common_MediaPath(ptr), FMOD_3D, 0, &pSound);
	ERRCHECK(result);
	result = pSound->set3DMinMaxDistance(0.5f * FMOD_DistanceFactor, 5000.0f * FMOD_DistanceFactor);
	ERRCHECK(result);
	// result = ptr->setMode(FMOD_LOOP_NORMAL);
	// ERRCHECK(result);

	m_Sounds.insert(std::pair<std::string, FMOD::Sound*>(str, pSound));
	return pSound;
}

void Sltn::playSound(FMOD::Sound* soundPtr)
{
	FMOD_VECTOR pos = { 0.0f * FMOD_DistanceFactor, 0.0f, 0.0f };
	FMOD_VECTOR vel = { 0.0f, 0.0f, 0.0f };
	playSound(soundPtr, pos, vel);
}
void Sltn::playSound(FMOD::Sound* soundPtr, FMOD_VECTOR pos, FMOD_VECTOR vel)
{
	if (soundPtr == nullptr) return;

	FMOD_RESULT      result;
	result = system->playSound(soundPtr, 0, true, &channel1);
	ERRCHECK(result);
	result = channel1->set3DAttributes(&pos, &vel);
	ERRCHECK(result);
	result = channel1->setPaused(false);
	ERRCHECK(result);
}

int Sltn::fmod_mainFunction()
{
	return 0;
	//FMOD::System    *system;
	//FMOD_RESULT      result;

	bool             listenerflag = true;

	//Load some sounds
	// sound1= getSound("./resources/PY7.wav");
	// sound2= getSound("./resources/RifleFire.wav");
	// sound3= getSound("./resources/FlamerActive.wav");

	// result = system->createSound(Common_MediaPath("./resources/PY7.wav"), FMOD_3D, 0, &sound1);
	// ERRCHECK(result);
	// result = sound1->set3DMinMaxDistance(0.5f * FMOD_DistanceFactor, 5000.0f * FMOD_DistanceFactor);
	// ERRCHECK(result);
	// result = sound1->setMode(FMOD_LOOP_NORMAL);
	// ERRCHECK(result);
	// 
	// result = system->createSound(Common_MediaPath("./resources/RifleFire.wav"), FMOD_3D, 0, &sound2);
	// ERRCHECK(result);
	// result = sound2->set3DMinMaxDistance(0.5f * FMOD_DistanceFactor, 5000.0f * FMOD_DistanceFactor);
	// ERRCHECK(result);
	// result = sound2->setMode(FMOD_LOOP_NORMAL);
	// ERRCHECK(result);
	// 
	// result = system->createSound(Common_MediaPath("./resources/FlamerActive.wav"), FMOD_SOFTWARE | FMOD_2D, 0, &sound3);
	// ERRCHECK(result);

	// Play sounds at certain positions

	// {
	// 	FMOD_VECTOR pos = { -10.0f * FMOD_DistanceFactor, 0.0f, 0.0f };
	// 	FMOD_VECTOR vel = {  0.0f, 0.0f, 0.0f };
	// 
	// 	result = system->playSound(sound1, 0, true, &channel1);
	// 	ERRCHECK(result);
	// 	result = channel1->set3DAttributes(&pos, &vel);
	// 	ERRCHECK(result);
	// 	result = channel1->setPaused(false);
	// 	ERRCHECK(result);
	// }
	// 
	// {
	// 	FMOD_VECTOR pos = { 15.0f * FMOD_DistanceFactor, 0.0f, 0.0f };
	// 	FMOD_VECTOR vel = { 0.0f, 0.0f, 0.0f };
	// 
	// 	result = system->playSound(sound2, 0, true, &channel2);
	// 	ERRCHECK(result);
	// 	result = channel2->set3DAttributes(&pos, &vel);
	// 	ERRCHECK(result);
	// 	result = channel2->setPaused(false);
	// 	ERRCHECK(result);
	// }

	//Main loop
	/*
	do
	{
	Common_Update();

	if (Common_BtnPress(BTN_ACTION1))
	{
	bool paused;
	channel1->getPaused(&paused);
	channel1->setPaused(!paused);
	}

	if (Common_BtnPress(BTN_ACTION2))
	{
	bool paused;
	channel2->getPaused(&paused);
	channel2->setPaused(!paused);
	}

	if (Common_BtnPress(BTN_ACTION3))
	{
	result = system->playSound(sound3, 0, false, &channel3);
	ERRCHECK(result);
	}

	if (Common_BtnPress(BTN_MORE))
	{
	listenerflag = !listenerflag;
	}

	if (!listenerflag)
	{
	if (Common_BtnDown(BTN_LEFT))
	{
	listenerpos.x -= 1.0f * FMOD_DistanceFactor;
	if (listenerpos.x < -24 * FMOD_DistanceFactor)
	{
	listenerpos.x = -24 * FMOD_DistanceFactor;
	}
	}

	if (Common_BtnDown(BTN_RIGHT))
	{
	listenerpos.x += 1.0f * FMOD_DistanceFactor;
	if (listenerpos.x > 23 * FMOD_DistanceFactor)
	{
	listenerpos.x = 23 * FMOD_DistanceFactor;
	}
	}
	}

	// ==========================================================================================
	// UPDATE THE LISTENER
	// ==========================================================================================
	{
	static float t = 0;
	static FMOD_VECTOR lastpos = { 0.0f, 0.0f, 0.0f };
	FMOD_VECTOR forward        = { 0.0f, 0.0f, 1.0f };
	FMOD_VECTOR up             = { 0.0f, 1.0f, 0.0f };
	FMOD_VECTOR vel;

	if (listenerflag)
	{
	listenerpos.x = (float)sin(t * 0.05f) * 24.0f * FMOD_DistanceFactor; // left right pingpong
	}

	// ********* NOTE ******* READ NEXT COMMENT!!!!!
	// vel = how far we moved last FRAME (m/f), then time compensate it to SECONDS (m/s).
	vel.x = (listenerpos.x - lastpos.x) * (1000 / INTERFACE_UPDATETIME);
	vel.y = (listenerpos.y - lastpos.y) * (1000 / INTERFACE_UPDATETIME);
	vel.z = (listenerpos.z - lastpos.z) * (1000 / INTERFACE_UPDATETIME);

	// store pos for next time
	lastpos = listenerpos;

	result = system->set3DListenerAttributes(0, &listenerpos, &vel, &forward, &up);
	ERRCHECK(result);

	t += (30 * (1.0f / (float)INTERFACE_UPDATETIME));    // t is just a time value .. it increments in 30m/s steps in this example
	}

	result = system->update();
	ERRCHECK(result);

	// Create small visual display.
	char s[80] = "|.............<1>......................<2>.......|";
	s[(int)(listenerpos.x / FMOD_DistanceFactor) + 25] = 'L';

	Common_Draw("==================================================");
	Common_Draw("3D Example.");
	Common_Draw("Copyright (c) Firelight Technologies 2004-2013.");
	Common_Draw("==================================================");
	Common_Draw("");
	Common_Draw("Press %s to toggle sound 1 (16bit Mono 3D)", Common_BtnStr(BTN_ACTION1));
	Common_Draw("Press %s to toggle sound 2 (8bit Mono 3D)", Common_BtnStr(BTN_ACTION2));
	Common_Draw("Press %s to play a sound (16bit Stereo 2D)", Common_BtnStr(BTN_ACTION3));
	Common_Draw("Press %s or %s to move listener in still mode", Common_BtnStr(BTN_LEFT), Common_BtnStr(BTN_RIGHT));
	Common_Draw("Press %s to toggle listener auto movement", Common_BtnStr(BTN_MORE));
	Common_Draw("Press %s to quit", Common_BtnStr(BTN_QUIT));
	Common_Draw("");
	Common_Draw(s);

	Common_Sleep(INTERFACE_UPDATETIME - 1);
	} while (!Common_BtnPress(BTN_QUIT));
	*/
	// Shut down

	return -1;

	// See destructor
	// result = sound1->release();
	// ERRCHECK(result);
	// result = sound2->release();
	// ERRCHECK(result);
	// result = sound3->release();
	// ERRCHECK(result);
	// 
	// result = system->close();
	// ERRCHECK(result);
	// result = system->release();
	// ERRCHECK(result);
	// 
	// Common_Close();

	return 0;
} // */



















b2Vec2 to_b2Vec2(const sf::Vector2f& vec){
	return b2Vec2(vec.x, vec.y);
}

b2Vec2 to_b2Vec2(const sf::Vector2<int>& vec){
	return b2Vec2((float)vec.x, (float)vec.y);
}

b2Vec2 to_b2Vec2(const sf::Vector2<unsigned int>& vec){
	return b2Vec2((float)vec.x, (float)vec.y);
}

b2Vec2 to_b2Vec2(const unsigned int x, unsigned int y){
	return b2Vec2((float)x, (float)y);
}


sf::Vector2<float> to_Vector2(const b2Vec2 vec){
	return sf::Vector2<float>(vec.x, vec.y);
}


std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
	elems.clear();
	std::stringstream ss(s);
	std::string item;
	while (std::getline(ss, item, delim)) {
		elems.push_back(item);
	}
	return elems;
}