#include "Gameplay.h"
#include "Enemy.h"
#include "SpaceStation.h"
//#include <math.h>



Gameplay::Gameplay(void) :
m_player(Player(sf::Vector2f(50, 50)))
, m_View(sf::FloatRect(0, 0, 100, 60))
, m_mouseTimer(0)
{

	m_bollekesVec.reserve(3000);











	//texture.setRepeated(true); tiled
	backgroundSpr.setTexture(sltn::getInst().GetTexture("resources/space.jpg"));
	backgroundSpr.setScale(0.2f, 0.2f);
	backgroundSpr.setPosition(sf::Vector2f(-99 / 2, -99 / 2));
	auto prevRect = backgroundSpr.getTextureRect();
	prevRect.width = 99999;
	prevRect.height = 99999;
	backgroundSpr.setTextureRect(prevRect);

	m_entities.push_back(new SpaceStation(sf::Vector2f(300, 40)));

	const unsigned int arrSize = 3;
	Enemy* arr[3];
	arr[0] = new Enemy(sf::Vector2f(500, 300));
	arr[1] = new Enemy(sf::Vector2f(80, 50));
	arr[2] = new Enemy(sf::Vector2f(200, 200));
	m_bollekesVec.push_back(arr[0]);
	m_bollekesVec.push_back(arr[1]);
	m_bollekesVec.push_back(arr[2]);

	for (int i = 0; i < arrSize; ++i){
		MakeCircle(arr[i]->getPosition(), 6, 3);
		MakeCircle(arr[i]->getPosition(), 7, 3);
	}

	for (unsigned int i = 0; i < 100; i++)
	{
		//auto pos= sf::Vector2f((float)(rand()%sltn::getInst().m_ScreenSize.x/8) , 
		//    (float)(rand()%sltn::getInst().m_ScreenSize.y));
		auto pos = sf::Vector2f((float)(rand() % 500), (float)(rand() % 200));
		m_bollekesVec.push_back(new Ball(pos));
	}

	sf::Vector2f place(20.0f, 20.0f);

	for (float len = 7.0f; len < 25; len += 3.0f){
		MakeCircle(place, len);
	}


}


Gameplay::~Gameplay()
{
	for (auto& ptr : m_bollekesVec){
		delete ptr;
		ptr = nullptr;
	}
	for (auto& ptr : m_entities){
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

// engine Logic
void ConnectBodys(b2Body* bodyA, b2Body* bodyB){
	if (AreLinqued(bodyB, bodyA)) return;
	if (AreLinqued(bodyA, bodyB)) return;

	auto diffVec = (bodyA->GetPosition() - bodyB->GetPosition());
	b2RevoluteJointDef jd; //b2RevoluteJointDef b2WeldJoint  b2DistanceJointDef
	jd.bodyA = bodyA;
	jd.bodyB = bodyB;
	//jd.referenceAngle= std::atan2f(diffVec.y, diffVec.x); // radialen
	jd.referenceAngle = bodyB->GetAngle();
	//jd.localAnchorA.Set(0,0);//+sqrt(squared)/2);
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
	sltn::getInst().m_world->CreateJoint(&jd);

	auto udA = (UserData*)(bodyA->GetUserData());
	auto udB = (UserData*)(bodyB->GetUserData());

	if (CountJoints(bodyA) == 0){
		udA->creator->setFilterGroup(bodyB->GetFixtureList()->GetFilterData().groupIndex);
	}
	else{
		udB->creator->setFilterGroup(bodyA->GetFixtureList()->GetFilterData().groupIndex);
	}

	(udB)->isConectedToCluster = true;
}

bool Gameplay::TryConnect()
{


	vector<ChosenBody> chosenBodys;
	for (auto& ballA : m_bollekesVec){
		b2Body* bodyA = ballA->GetB2Body();
		if (((UserData*)bodyA->GetUserData())->isConectedToCluster != true) continue;

		for (auto& ballB : m_bollekesVec){

			b2Body* bodyB = ballB->GetB2Body();
			auto diffVec = (bodyA->GetPosition() - bodyB->GetPosition());
			auto squared = diffVec.LengthSquared();

			//if( ((UserData*)bodyB->GetUserData() )->isConectedToCluster == true ) continue;

			if (squared > 50 && squared<30) continue;

			auto chosenBody = ChosenBody(bodyA, bodyB, squared);
			if (chosenBody.numOfJoints >4) continue;

			chosenBodys.push_back(chosenBody);

		}
	}
	if (chosenBodys.size() <= 0) return false;

	// if( chosenBodys.size()<2 ) 
	float minDistance = 9999999.0f;
	ChosenBody cb = ChosenBody();
	for (auto chosenBody : chosenBodys)
	{
		auto distanceSqr = chosenBody.distanceSquared;
		if (distanceSqr < minDistance){
			minDistance = distanceSqr;
			cb = chosenBody;
		}
	}
	ConnectBodys(cb.bodyA, cb.bodyB);
	return true;
}

// GameplayLogic
void Gameplay::Connect(b2Body* bodyA, b2Body* bodyB){
	if (!(bodyA && bodyB)) return;

	auto diffVec = (bodyA->GetPosition() - bodyB->GetPosition());
	auto squared = diffVec.LengthSquared();
	if (squared > Ball::semiGlobal_minDistanceSquared) return;

	if (CountJoints(bodyA) > 10) return;

	ConnectBodys(bodyA, bodyB);
}











void Gameplay::Tick(const float deltaTime)
{
	//if (sf::Keyboard::isKeyPressed(sf::Keyboard::X)){
	//    if( m_player.GetB2Body() ==nullptr ) return;
	//    sltn::getInst().m_world->DestroyJoint(
	//        m_player.GetB2Body()->GetJointList()->joint);
	//    m_player.GetB2Body()->GetJointList()->joint = nullptr;
	//}
	m_mouseTimer += deltaTime;

	// m_View.setSize((float)sltn::getInst().m_ScreenSize.x,(float)sltn::getInst().m_ScreenSize.y);


	for (auto* jointIt = sltn::getInst().m_world->GetJointList(); jointIt; jointIt = jointIt->GetNext()){
		b2Vec2 reactionForce = jointIt->GetReactionForce(1 / deltaTime);
		float forceModuleSq = reactionForce.LengthSquared();
		if (forceModuleSq > 12000 * 12000)
			sltn::getInst().EnqueDestroyBody(jointIt);
		//sltn::getInst().m_world->DestroyJoint(jointIt);
	}

	auto worldPos = sltn::getInst().GetMousePos();

	if (m_mouseTimer > 0.5f && sf::Mouse::isButtonPressed(sf::Mouse::Left)){
		m_mouseTimer = 0;



		// - sf::Vector2i( viewrect.x, viewrect.y);
		auto force = to_b2Vec2(worldPos) - b2Vec2(m_player.getPosition().x, m_player.getPosition().y);
		auto len = force.Length();
		force.x /= len;
		force.y /= len;
		//force*=10.0f;

		auto bullet = new Bullet(m_player.getPosition() + to_Vector2(force)
			, atan2(force.y, force.x), true);

		force *= 65000.0f;
		bullet->GetB2Body()->ApplyForceToCenter(force);
		m_bulletVec.push_front(bullet);
		//m_bulletVec.remove(bullet);
	}
	if (m_mouseTimer > 0.01f && sf::Mouse::isButtonPressed(sf::Mouse::Middle)){
		m_mouseTimer = 0;

		// - sf::Vector2i( viewrect.x, viewrect.y);
		auto force = to_b2Vec2(worldPos) - b2Vec2(m_player.getPosition().x, m_player.getPosition().y);
		auto len = force.Length();
		force.x /= len;
		force.y /= len;
		//force*=10.0f;

		auto bullet = new Bullet(m_player.getPosition() + to_Vector2(force)
			, atan2(force.y, force.x));
		force *= 45000.0f;
		bullet->GetB2Body()->ApplyForceToCenter(force);
		m_bulletVec.push_front(bullet);
		//m_bulletVec.remove(bullet);
	}
	if (m_mouseTimer > 0.1f && sf::Mouse::isButtonPressed(sf::Mouse::Right)){
		m_mouseTimer = 0;

		auto ball = new Ball(worldPos);
		ball->setTexture(sltn::getInst().GetTexture("resources/blue-sphere_512.png"));
		m_bollekesVec.push_back(ball);

		Connect(m_player.GetB2Body(), m_bollekesVec.back()->GetB2Body());

		for (auto ball1 : m_bollekesVec){
			if (ball1 == nullptr) continue;
			//if( ( (UserData*)ball1.GetB2Body()->GetUserData() )->isConectedToCluster == false ) continue;
			Connect(ball1->GetB2Body(), m_bollekesVec.back()->GetB2Body());
		}

		//b2Body* bodyA= m_player.GetB2Body(); // m_playe
		//b2Body* bodyB= m_bollekesVec.back()->GetB2Body(); // ball

	}

	m_player.Tick(deltaTime);

	for (auto& object : m_bollekesVec)
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
		TryConnect();

	m_View.setCenter(m_player.getPosition());
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
	unsigned int bCount = sltn::getInst().m_world->GetBodyCount();
	sf::VertexArray vertexArray(sf::Quads, bCount * 4);
	for (b2Body* body = sltn::getInst().m_world->GetBodyList(); body; body = body->GetNext()){
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
	//        m_player.getPosition(), sf::Color::Yellow ));
	//    vertexArray.append(sf::Vertex(
	//        object.getPosition(), sf::Color::Red ));
	//}
	window.draw(vertexArray);

	vertexArray.clear();



	for (auto& object : m_bollekesVec)
	if (object != nullptr)
		window.draw(*object);

	for (auto& object : m_entities)
	if (object != nullptr)
		window.draw(*object);


	window.draw(m_player);

	for (auto object : m_bulletVec)
		window.draw(*object);


	int counter = 0;
	sf::VertexArray spriteVertexArray(sf::PrimitiveType::Quads, 4U * m_SpriteAnimationList.size());

	const sf::Texture* tex = nullptr;

	for (auto& object : m_SpriteAnimationList){
		if (object == nullptr) continue;
		if (tex == nullptr) tex = object->getTexture();
		float x = tex->getSize().y;
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
}
