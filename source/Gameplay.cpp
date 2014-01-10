#include "Gameplay.h"

#include "Ball.h"
#include "BallBase.h" //
#include "Bullet.h"
#include "Enemy.h"
#include "Player.h"
#include "SpaceStation.h"
#include "SpriteAnimation.h"
#include "VertexFigure.h"
#include "SpawnPoint.h"

#include "BgElement.h"
#include "Pickup.h"
#include "pugiXml\pugixml.hpp"
//using namespace pugi;
//#include <math.h>

Gameplay* Gameplay::instance = nullptr;

Gameplay& Gameplay::getInst() // get the singleton reference
{
	//static Gameplay    instance; // Guaranteed to be destroyed.
	// Instantiated on first use.
	if (instance == nullptr)instance = new Gameplay();
	return *instance;
}

Gameplay::Gameplay(void) :
m_player(nullptr)
, m_View(sf::FloatRect(0, 0, 100, 60))
, m_mouseTimer(0), m_Font(), m_NrOfKills()
{
	m_player = new Player(sf::Vector2f(70, 50));
	m_player->Initialize();

	m_Font.loadFromFile("C:/Windows/Fonts/Arial.TTF");

	m_BallsToAdd.reserve(3000); // not performant
	m_SpawnPointVec.reserve(5);
	m_Balls.reserve(3000);

	auto bgSound = Sltn::getInst().getSound("");
	Sltn::getInst().playSound(bgSound);

	// m_entities.push_back(new VertexFigure(sf::Vector2f(10, 10)));

	//texture.setRepeated(true); tiled
	backgroundSpr.setTexture(Sltn::getInst().GetTexture("resources/space.jpg"));
	backgroundSpr.setScale(0.2f, 0.2f);
	backgroundSpr.setPosition(sf::Vector2f(-99 / 2, -99 / 2));
	auto prevRect = backgroundSpr.getTextureRect();
	prevRect.width = 99999;
	prevRect.height = 99999;
	backgroundSpr.setTextureRect(prevRect);

	EnqueueAddToList(new SpaceStation(sf::Vector2f(360, 118)));
	//m_entities.push_back();

	const unsigned int arrSize = 2;
	m_SpawnPointVec.push_back(new SpawnPoint(sf::Vector2f(50, 50)));

	// sf::Vector2f place(20.0f, 20.0f);
	// for (float len = 7.0f; len < 25; len += 3.0f){
	// 	MakeCircle(place, len);
	// }

	LoadInktscapeFile(("resources/LevelFile.svg"));
}


Gameplay::~Gameplay()
{
	ApplyAddToQueue();
	ApplyRemoveFrom();
	for (auto& ptr : m_entities){
		delete ptr;
		ptr = nullptr;
	}
	for (auto& ptr : m_Balls){
		delete ptr;
		ptr = nullptr;
	}
	for (auto& ptr : m_bulletVec){
		delete ptr;
		ptr = nullptr;
	}
	for (auto& ptr : m_SpriteAnimationList){
		delete ptr;
		ptr = nullptr;
	}
	// To be sure
	ApplyAddToQueue();
	ApplyRemoveFrom();
	instance = nullptr;
}



void Gameplay::RemoveFrom(entityBase* entity){
	for (unsigned int i = 0; i < m_entities.size(); ++i){
		if (m_entities[i] == entity) {
			delete m_entities[i];
			m_entities[i] = nullptr;
			return;
		}
	}
	for (auto& ptr : m_Balls){
		if (ptr == entity) {
			delete entity;
			ptr = nullptr;
			return;
		}
	}
	for (auto& ptr : m_SpriteAnimationList){
		if (ptr == entity) {
			//delete entity;
			ptr = nullptr;
			return;
		}
	}
	//for (unsigned int i = 0; i < m_bulletVec.size(); ++i){
	//	if (m_bulletVec[i] == entity) {
	//		//delete m_bulletVec[i];
	//		m_bulletVec[i] = nullptr;
	//		return;
	//	}
	//}
	for (auto& ptr : m_bulletVec){
		if (ptr == entity) {
			ptr = nullptr;
			return;
		}
	}
	//Sltn::getInst().RemoveBody(entity.)
}


b2Vec2 Gameplay::GetPlayerPos(){
	if (m_player && m_player->GetB2Body())
		return m_player->GetB2Body()->GetPosition();
	else
		return b2Vec2(108, 108);
}

bool AreLinqued(b2Body* bodyA, b2Body* bodyB)
{
	if (bodyA == bodyB) return true;
	for (b2JointEdge* j = bodyB->GetJointList(); j; j = j->next){
		if (j->other == bodyA)
			return true;
	}
	return false;
}

unsigned int CountJoints(b2Body* body){
	unsigned int returner = 0;
	for (b2JointEdge* j = body->GetJointList(); j; j = j->next)
		++returner;
	return returner;
}

struct ChosenBody{
	ChosenBody() :bodyA(nullptr), bodyB(nullptr), distanceSquared(108888.0f), numOfJoints(0)
	{}
	ChosenBody(
		b2Body* bodyA,
		b2Body* bodyB,
		float distanceA
		//unsigned int numOfJointsA
		) : bodyA(bodyA), bodyB(bodyB), distanceSquared(distanceA), numOfJoints(0)
	{
		if (bodyA)
			numOfJoints = CountJoints(bodyA);
	}
	b2Body* bodyA;
	b2Body* bodyB;
	float distanceSquared;
	unsigned int numOfJoints; // get calculated for A
};

bool CCW(b2Vec2 p1, b2Vec2 p2, b2Vec2 p3) {
	float a = p1.y; float b = p1.x;
	float c = p2.y; float d = p2.x;
	float e = p3.y; float f = p3.x;
	return (f - b) * (c - a) > (d - b) * (e - a);
}

bool isIntersect(b2Vec2 p1, b2Vec2 p2, b2Vec2 q3, b2Vec2 q4) {
	float side1 = (p2.x - p1.x)*(q3.y - p2.y) < (p2.y - p1.y)*(q3.x - p2.x);
	float side2 = (p2.x - p1.x)*(q4.y - p2.y) < (p2.y - p1.y)*(q4.x - p2.x);
	return (side1 == side2);
	// (Fx - Ex)(Py - Fy) - (Fy - Ey)(Px - Fx);
	// (Fx - Ex)(Qy - Fy) - (Fy - Ey)(Qx - Fx);
}

bool FileExists(const string& filename)
{
	if (ifstream(filename.c_str())){ //openFile
		return true;
	}
	return false; // openFile zit in de stack en word dus nu gedelete
}
bool FileExists(const wstring& filename)
{
	if (wifstream(filename.c_str())){ //openFile
		return true;
	}
	return false; // openFile zit in de stack en word dus nu gedelete
}

// recursevely infect te cluster
//void SpreadFilterGroup(b2Body* bodyA, int16 groupIndex){
//	if (bodyA->GetFixtureList()->GetFilterData().groupIndex == groupIndex) return;
//	bodyA->GetFixtureList()->SetFilterData(b2Filter(groupIndex));
//
//	for (auto joint = bodyA->GetJointList(); joint; joint = joint->next){
//		SpreadFilterGroup(joint->other, groupIndex);
//	}
//}
void SpreadFilterGroup(b2Body* bodyA, const uint16 categoryBits, const uint16 maskBits){
	b2Filter filter = bodyA->GetFixtureList()->GetFilterData();
	if (filter.categoryBits == categoryBits &&
		filter.maskBits == maskBits) return;


	filter.categoryBits = categoryBits;
	filter.maskBits = maskBits;

	bodyA->GetFixtureList()->SetFilterData(filter);

	for (auto joint = bodyA->GetJointList(); joint; joint = joint->next){
		SpreadFilterGroup(joint->other, categoryBits, maskBits);
	}
}

// GameplayLogic
bool ConnectTry(b2Body* bodyA, b2Body* bodyB){
	if (bodyA == bodyB) return false;
	if (!(bodyA && bodyB)) return false;

	auto diffVec = (bodyA->GetPosition() - bodyB->GetPosition());
	auto squared = diffVec.LengthSquared();
	if (squared > BallBase::semiGlobal_minDistanceSquared) return false;
	if (squared<4)
		return false;

	if (CountJoints(bodyA) > 5) return false;
	if (CountJoints(bodyB) > 5) return false;

	ConnectBodys(bodyA, bodyB);
	return true;
}

// engine Logic
bool ConnectBodys(b2Body* bodyA, b2Body* bodyB){
	if (AreLinqued(bodyB, bodyA)) return false;
	if (AreLinqued(bodyA, bodyB)) return false;

	// bodyB is the newkid

	auto diffVec = (bodyA->GetPosition() - bodyB->GetPosition());
	b2RevoluteJointDef jd; //b2RevoluteJointDef b2WeldJoint  b2DistanceJointDef b2Vec2(0, 0)
	jd.Initialize(bodyA, bodyB, bodyA->GetPosition());
	/*
	jd.bodyA = bodyA;
	jd.bodyB = bodyB;
	//jd.referenceAngle= std::atan2f(diffVec.y, diffVec.x); // radialen
	jd.referenceAngle = -bodyA->GetAngle();
	//jd.localAnchorA.Set(0, 0);//+sqrt(squared)/2);
	jd.localAnchorB.Set(diffVec.x, diffVec.y);//-sqrt(squared)/2);
	//jd.localAnchorA.Set(bodyB->GetPosition().x, bodyB->GetPosition().y);//+sqrt(squared)/2);
	//jd.localAnchorB.Set(bodyA->GetPosition().x, bodyA->GetPosition().y);//-sqrt(squared)/2);
	//jd.frequencyHz = 20.0f;
	//jd.referenceAngle= 0; //atan2f(diffVec.x, diffVec.y);
	//jd.dampingRatio=1;
	//jd.collideConnected= false;
	//jd.length=5;
	//jd.referenceAngle = 0.0f;
	//jd.motorSpeed = 0.05f * b2_pi;
	//jd.maxMotorTorque = 1e8f;
	//jd.enableMotor = true;
	*/
	Sltn::getInst().m_world->CreateJoint(&jd);

	auto udA = (UserData*)(bodyA->GetUserData());
	auto udB = (UserData*)(bodyB->GetUserData());

	(udA)->isConectedToCluster = true;
	(udB)->isConectedToCluster = true;

	//return true;

	auto filterA = bodyA->GetFixtureList()->GetFilterData();
	auto filterB = bodyB->GetFixtureList()->GetFilterData();

	//if (filterA.groupIndex != filterB.groupIndex) {
	//	if (filterA.groupIndex != 0 && filterB.groupIndex != 0)
	//		return false; // Both a different exotic value
	//}

	if (filterA.categoryBits == 0x1){
		//udA->creator->setFilterGroup(filterB.groupIndex);
		SpreadFilterGroup(bodyA, filterB.categoryBits, filterB.maskBits);
	}
	else{
		//udB->creator->setFilterGroup(filterA.groupIndex);
		//SpreadFilterGroup(bodyB, filterA.groupIndex);
		SpreadFilterGroup(bodyB, filterA.categoryBits, filterA.maskBits);
	}

	return true;
}


void Gameplay::ConnectWithOthers(BallBase* ballNew)
{
	assert(ballNew);
	//if (ballNew == nullptr)return;
	b2Body* bodyA = ballNew->GetB2Body();
	if (bodyA == nullptr)return;

	for (auto ballB : m_Balls){

		if (ballB == nullptr) continue;
		b2Body* bodyB = ballB->GetB2Body();
		if (bodyB == nullptr) continue;


		ConnectTry(bodyA, bodyB);
	}
}


//bool Gameplay::TryConnect()
//{
//
//
//	vector<ChosenBody> chosenBodys;
//	for (auto& ballA : m_Balls){
//
//		if (ballA == nullptr)continue;
//		b2Body* bodyA = ballA->GetB2Body();
//		if (bodyA == nullptr)continue;
//
//		if (((UserData*)bodyA->GetUserData())->isConectedToCluster != true) continue;
//
//		for (auto ballB : m_Balls){
//
//			if (ballB == nullptr)continue;
//			b2Body* bodyB = ballB->GetB2Body();
//			if (bodyB == nullptr)continue;
//			auto diffVec = (bodyA->GetPosition() - bodyB->GetPosition());
//			auto squared = diffVec.LengthSquared();
//
//			//if( ((UserData*)bodyB->GetUserData() )->isConectedToCluster == true ) continue;
//
//			if (squared > 50 && squared<30) continue;
//
//			auto chosenBody = ChosenBody(bodyA, bodyB, squared);
//			if (chosenBody.numOfJoints >4) continue;
//
//			chosenBodys.push_back(chosenBody);
//		}
//	}
//	if (chosenBodys.size() <= 0) return false;
//
//	// if( chosenBodys.size()<2 ) 
//	float minDistance = 9999999.0f;
//	ChosenBody cb = ChosenBody();
//	for (auto chosenBody : chosenBodys)
//	{
//		auto distanceSqr = chosenBody.distanceSquared;
//		if (distanceSqr < minDistance){
//			minDistance = distanceSqr;
//			cb = chosenBody;
//		}
//	}
//	ConnectBodys(cb.bodyA, cb.bodyB);
//	return true;
//}



void Gameplay::Tick(const float deltaTime)
{
	//if (sf::Keyboard::isKeyPressed(sf::Keyboard::X)){
	//    if( m_player->GetB2Body() ==nullptr ) return;
	//    Sltn::getInst().m_world->DestroyJoint(
	//        m_player->GetB2Body()->GetJointList()->joint);
	//    m_player->GetB2Body()->GetJointList()->joint = nullptr;
	//}
	m_mouseTimer += deltaTime;

	for (auto ptr : m_SpawnPointVec)
		ptr->Tick(deltaTime);

	// m_View.setSize((float)Sltn::getInst().m_ScreenSize.x,(float)Sltn::getInst().m_ScreenSize.y);


	for (auto* jointIt = Sltn::getInst().m_world->GetJointList(); jointIt; jointIt = jointIt->GetNext()){
		b2Vec2 reactionForce = jointIt->GetReactionForce(1 / deltaTime);
		float forceModuleSq = reactionForce.LengthSquared();
		if (forceModuleSq > 11000 * 11000)
			Sltn::getInst().EnqueDestroyPhysicsEntity(jointIt);
		//Sltn::getInst().m_world->DestroyJoint(jointIt);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
		LoadInktscapeFile(("resources/LevelFile.svg"));

	m_player->Tick(deltaTime);

	for (auto& object : m_Balls)
	if (object != nullptr)
		object->Tick(deltaTime);

	for (auto& object : m_entities)
	if (object != nullptr)
		object->Tick(deltaTime);

	for (auto object : m_bulletVec)
	if (object != nullptr)
		object->Tick(deltaTime);

	for (auto object : m_SpriteAnimationList)
	if (object != nullptr)
		object->Tick(deltaTime);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
		sf::sleep(sf::milliseconds(50));
	//TryConnect();

	ApplyRemoveFrom();
	ApplyAddToQueue();

	m_View.setCenter(m_player->getPosition());
}

void AddThickLine(sf::VertexArray& vertices, const sf::Vector2f& point1, const sf::Vector2f& point2)
{
	auto thickness = 0.5f;

	sf::Vector2f direction = point2 - point1;
	sf::Vector2f unitDirection = direction / std::sqrt(direction.x*direction.x + direction.y*direction.y);
	sf::Vector2f unitPerpendicular(-unitDirection.y, unitDirection.x);

	sf::Vector2f offset = (thickness / 2.f)*unitPerpendicular;

	vertices.append(sf::Vertex(point1 + offset, sf::Color::Blue));
	vertices.append(sf::Vertex(point2 + offset, sf::Color::Blue));
	vertices.append(sf::Vertex(point2 - offset, sf::Color::Blue));
	vertices.append(sf::Vertex(point1 - offset, sf::Color::Blue));

	// vertices[0].position = point1 + offset;
	// vertices[1].position = point2 + offset;
	// vertices[2].position = point2 - offset;
	// vertices[3].position = point1 - offset;
}
void Gameplay::PaintGui(sf::RenderWindow& window)
{
	window.setView(sf::View());



	auto text = sf::Text(sf::String((L"Nr of kills ")) + std::to_wstring(m_NrOfKills), m_Font, 30);
	text.setPosition(sf::Vector2f(10, 10));
	text.setColor(sf::Color(0, 0, 0));

	auto marging = sf::Vector2f(5, 5);
	auto rect = sf::RectangleShape(
		sf::Vector2f(text.getGlobalBounds().width, text.getGlobalBounds().height)
		+ marging*2.f);
	rect.setPosition(text.getPosition() - marging);
	rect.setFillColor(sf::Color::Blue);

	window.draw(rect);
	window.draw(text);
}

void Gameplay::Paint(sf::RenderWindow& window)
{

	window.setView(m_View);


	//sf::RenderTexture* texture= new sf::RenderTexture(); texture->create(4096, 2048);
	window.draw(backgroundSpr);



	//auto tex= sf::Texture();
	//tex.create(32,32);
	//tex.draw(backgroundSpr);

	auto ha = sf::RenderTexture();
	ha.pushGLStates();
	ha.draw(backgroundSpr);

	// Debug draw the connections
	unsigned int bCount = Sltn::getInst().m_world->GetBodyCount();
	sf::VertexArray vertexArray(sf::Quads, bCount * 4);
	for (b2Body* body = Sltn::getInst().m_world->GetBodyList(); body; body = body->GetNext()){
		for (b2JointEdge* j = body->GetJointList(); j; j = j->next){

			AddThickLine(vertexArray,
				sf::Vector2f(j->other->GetPosition().x, j->other->GetPosition().y),
				sf::Vector2f(body->GetPosition().x, body->GetPosition().y)
				);
			// vertexArray.append(sf::Vertex(
			// 	sf::Vector2f(j->other->GetPosition().x,j->other->GetPosition().y), sf::Color::Blue ));
			// vertexArray.append(sf::Vertex(
			// 	sf::Vector2f(body->GetPosition().x,body->GetPosition().y), sf::Color::Red ));
		}
	}
	// BulletLines
	//for( auto& object : m_bulletVec ){
	//    vertexArray.append(sf::Vertex(
	//        m_player->getPosition(), sf::Color::Yellow ));
	//    vertexArray.append(sf::Vertex(
	//        object.getPosition(), sf::Color::Red ));
	//}
	window.draw(vertexArray);

	vertexArray.clear();

	for (auto object : m_Drawables)
		window.draw(*object);

	for (auto& object : m_Balls)
	if (object != nullptr)
		window.draw(*object);

	for (auto& object : m_entities)
	if (object != nullptr)
		window.draw(*object);


	window.draw(*m_player);

	for (auto object : m_bulletVec)
		window.draw(*object);


	int counter = 0;
	//size_t 
	sf::VertexArray spriteVertexArray(sf::PrimitiveType::Quads, (unsigned int)(4U * m_SpriteAnimationList.size()));

	const sf::Texture* tex = nullptr;

	for (auto& object : m_SpriteAnimationList){
		if (object == nullptr) continue;
		if (tex == nullptr) tex = object->getTexture();
		float x = (float)tex->getSize().y;
		//window.draw(*object);
		float s = 3.0f; // size
		spriteVertexArray.append(sf::Vertex(object->getPosition() + sf::Vector2f(-s, -s), sf::Color::White, sf::Vector2f(0, 0)));
		spriteVertexArray.append(sf::Vertex(object->getPosition() + sf::Vector2f(+s, -s), sf::Color::White, sf::Vector2f(x, 0)));
		spriteVertexArray.append(sf::Vertex(object->getPosition() + sf::Vector2f(+s, +s), sf::Color::White, sf::Vector2f(x, x)));
		spriteVertexArray.append(sf::Vertex(object->getPosition() + sf::Vector2f(-s, +s), sf::Color::White, sf::Vector2f(0, x)));
		++counter;

	}
	if (spriteVertexArray.getVertexCount() > 0){
		if (tex != nullptr){
			auto it = m_SpriteAnimationList.begin();
			//sf::Transform::Identity
			auto rs = sf::RenderStates(tex); // (*it)->getTexture()
			// rs.blendMode= sf::BlendMode::BlendAdd; //,m_View.getTransform, 
			window.draw(spriteVertexArray, rs);
		}
	}


	PaintGui(window);
	window.setView(m_View);
}

void Gameplay::MakeCircle(sf::Vector2f place, float len, float distance = BallBase::semiGlobal_minDistance){
	float pi = 3.14159265f;
	float contour = 2.0f *len* pi;
	for (float radial = 0; radial < 2.0f * pi; radial += 2.0f*pi / (contour / distance)){

		auto pos = sf::Vector2f(len*cos(radial), len*sin(radial));
		pos += place;
		EnqueueAddToList(new Ball(pos));
		//m_Balls.push_back(new BallBase(pos));

	}
}
void Gameplay::StickBodyToWorld(b2Body* body){

	for (auto ball1 : m_Balls){
		if (ball1 == nullptr) continue;
		// connect the last ball with the rest
		ConnectTry(ball1->GetB2Body(), body);
	}
}

void Gameplay::LoadInktscapeFile(const char* HitregionsFile)
{
	//God::GetSingleton()->AddHitStuk(sf::Vector2f(10.0,100.0),  ("Level"));

	//TCHAR* HitregionsFile=  ("resources/Level/Level_Hitregions.svg"); // Level_Hitregions
	if (!FileExists(HitregionsFile)) return;

	pugi::xml_document xmlDoc;
	pugi::xml_parse_result result = xmlDoc.load_file(HitregionsFile);

	for (auto ptr : m_Drawables)
		delete ptr;
	m_Drawables.clear();


	//xmlDoc.child( ("g"));
	for (pugi::xml_node elementNode = xmlDoc.child(("svg")).child(("g")).first_child(); elementNode; elementNode = elementNode.next_sibling())
	{
		b2Vec2 pos(0, 0);
		b2Vec2 radius(0, 0);

		auto sodipodi = elementNode.attribute(("sodipodi:cx"));
		if (sodipodi != 0){
			pos.x = elementNode.attribute(("sodipodi:cx")).as_float();
			pos.y = elementNode.attribute(("sodipodi:cy")).as_float();

			radius.x = elementNode.attribute(("sodipodi:rx")).as_float();
			radius.y = elementNode.attribute(("sodipodi:ry")).as_float();
		}

		if (elementNode.attribute(("x")) != 0){
			pos.x += elementNode.attribute(("x")).as_float();
			pos.y += elementNode.attribute(("y")).as_float();
		}

		// transform/translate Matrix/snizzle  //////////
		sf::Transformable matRotate;// matRotate.SetAsIdentity();
		auto transformAttr = elementNode.attribute("transform");
		if (transformAttr != 0){
			string strMat = transformAttr.as_string();
			strMat = strMat.substr(strMat.find_last_of(("(")) + 1, 999);
			strMat = strMat.substr(0, strMat.find_last_of((")")));

			vector<float> readArrfloats;
			std::vector<string> elements;
			split(strMat, ',', elements);
			for (string s : elements)
				readArrfloats.push_back(std::stof(s.c_str()));

			string beginStr(transformAttr.value(), 10);
			if (beginStr.find("translate") == 0){
				pos.x += readArrfloats.at(0);
				pos.y += readArrfloats[1];
			}
			else if (beginStr.find("matrix") == 0){
				// not supported
			}
			//matRotate = sf::Transformable(strMat);
		}
		// ptr->setRotation(rand() % 100 - 200); // not supported

		if ((string)elementNode.name() == (string)"path"){
			auto attribute = elementNode.attribute(("sodipodi:type"));
			if (attribute != 0 && (string(attribute.value()) == string(("arc")))){
				auto ptr = new BgElement();

				try{
					auto colorStr = string(elementNode.attribute(("style")).value(), 6, 6);
					unsigned int r, g, b;
					stringstream ss;
					ss << std::hex << colorStr.substr(0, 2); ss >> r; ss.clear();
					ss << std::hex << colorStr.substr(2, 2); ss >> g; ss.clear();
					ss << std::hex << colorStr.substr(4, 2); ss >> b; ss.clear();
					ptr->setFillColor(sf::Color(r, g, b));
				}
				catch (int e){}

				ptr->setTexture(&Sltn::getInst().GetTexture("resources/mars.jpg"));

				ptr->SetAsOval(
					pos, radius
					);
				m_Drawables.push_back(ptr);
				continue;
			}
		}

		// Name ////////////
		string nameStr = elementNode.attribute(("xlink:href")).as_string();
		nameStr = nameStr.substr(nameStr.find_last_of(("/")) + 1, 999);
		nameStr = nameStr.substr(0, nameStr.find_last_of((".")));

		sf::Transformable matTotalTransform;



		if (nameStr.find(("pickup1")) != string::npos){ // Sprite of sprite
			m_Drawables.push_back(new Pickup(pos));
			continue;
		}
		else if (nameStr.find(("EnemySpawner")) != string::npos){
			m_SpawnPointVec.push_back(new SpawnPoint(to_Vector2(pos)));
			continue; // volgende item in de svg
		}
		else if (nameStr.find(("prite")) != string::npos){ // Sprite of sprite
			nameStr = nameStr.substr(0, nameStr.find_last_of(("_")));
			continue;
		}

		// Scale ////////////
		//sf::Transformable matScale; // matScale.SetAsIdentity();
		sf::Vector2f scaledSize, imageSize(108, 108);
		if (elementNode.attribute(("width")) != 0){
			scaledSize.x = elementNode.attribute(("width")).as_float();
			scaledSize.y = elementNode.attribute(("height")).as_float();
			float imageSizeX = 108, imageSizeY = 108;
			string file = ("./resources/level/") + nameStr + ("_sprite.png");
			//God::GetImageSizeEx(file.c_str(), &imageSizeX, &imageSizeY);
			imageSize = sf::Vector2f(imageSizeX, imageSizeY);
			matTotalTransform.scale(scaledSize.x / imageSizeX, scaledSize.y / imageSizeY);
			//matScale.setScale(scaledSize.x / imageSizeX, scaledSize.y / imageSizeY);
		}



		// matTotalTransform /////////////// *sf::Transformable::CreateTranslationMatrix(imageSize/2)
		// sf::Transformable matTotalTransform = matScale * matRotate * matPos;



		//God::GetSingleton()->AddHitStuk(matTotalTransform, nameStr);

	}
}



void Gameplay::ApplyAddToQueue(){
	//auto world = Sltn::getInst().m_world;
	for (auto ptr : m_entitiesToAdd){
		ptr->Initialize();
		m_entities.push_back(ptr);
	}
	m_entitiesToAdd.clear();

	for (unsigned int i = 0; i < m_BallsToAdd.size(); ++i){
		m_Balls.push_back(m_BallsToAdd[i]);
		m_BallsToAdd[i]->Initialize();
	}
	m_BallsToAdd.clear();
}
void  Gameplay::ApplyRemoveFrom(){
	for (auto ptr : m_entitiesRemoveFrom){
		RemoveFrom(ptr);
	}
	m_entitiesRemoveFrom.clear();
}