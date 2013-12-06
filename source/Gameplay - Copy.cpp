#include "Gameplay.h"


Gameplay::Gameplay(void):
    m_player(Player(sf::Vector2f(50,50)))
    ,m_View(sf::FloatRect(0, 0, 100, 60))
    ,m_mouseTimer(0)
{

    //texture.setRepeated(true); tiled
    backgroundSpr.setTexture(sltn::getInst().GetTexture("resources/space.jpg"));
    backgroundSpr.setScale(0.2f,0.2f);
    backgroundSpr.setPosition(sf::Vector2f(0, 0));

    m_bollekesVec.reserve(3000);
    for (unsigned int i = 0; i < 1000; i++)
    {
        auto pos= sf::Vector2f((float)(rand()%sltn::getInst().m_ScreenSize.x/8) , 
            (float)(rand()%sltn::getInst().m_ScreenSize.y));
        m_bollekesVec.push_back( new Ball(pos) );
    }

    m_player.setTexture(sltn::getInst().GetTexture("resources/Wheatley.png"));

}


Gameplay::~Gameplay(void)
{
}






bool AreLinqued(b2Body* bodyA,b2Body* bodyB)
{
    if( bodyA== bodyB ) return true;
    for(b2JointEdge* j =bodyB->GetJointList();j;j=j->next ){
        if( j->other == bodyA ) 
            return true;
    }
    return false;
}

unsigned int CountJoints(b2Body* body){
    unsigned int returner=0;
    for(b2JointEdge* j =body->GetJointList();j;j=j->next )
        ++returner;
    return returner;
}

struct ChosenBody{
    ChosenBody():bodyA(nullptr), bodyB(nullptr), distanceSquared(108888.0f), numOfJoints(0)
    {}
    ChosenBody(
        b2Body* bodyA,
        b2Body* bodyB,
        float distanceA
        //unsigned int numOfJointsA
        ):	bodyA(bodyA),bodyB(bodyB), distanceSquared(distanceA), numOfJoints(0)
    {
        if( bodyA )
            numOfJoints= CountJoints(bodyA);
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
    float side1= (p2.x - p1.x)*(q3.y - p2.y) < (p2.y - p1.y)*(q3.x - p2.x);
    float side2= (p2.x - p1.x)*(q4.y - p2.y) < (p2.y - p1.y)*(q4.x - p2.x);
    return (side1 == side2);
    // (Fx - Ex)(Py - Fy) - (Fy - Ey)(Px - Fx);
    // (Fx - Ex)(Qy - Fy) - (Fy - Ey)(Qx - Fx);
}

void ConnectBodys(b2Body* bodyA, b2Body* bodyB){
    if( AreLinqued(bodyB, bodyA) ) return;
    if( AreLinqued(bodyA, bodyB) ) return;

    auto diffVec= (bodyA->GetPosition()-bodyB->GetPosition());
    b2WeldJointDef jd; //b2RevoluteJointDef b2WeldJoint  b2DistanceJointDef
    jd.bodyA = bodyA;
    jd.bodyB = bodyB;
    //jd.referenceAngle= std::atan2f(diffVec.y, diffVec.x); // radialen
    jd.referenceAngle= bodyB->GetAngle();
    //jd.localAnchorA.Set(0,0);//+sqrt(squared)/2);
    jd.localAnchorB.Set(diffVec.x, diffVec.y);//-sqrt(squared)/2);
    //jd.localAnchorA.Set(bodyB->GetPosition().x, bodyB->GetPosition().y);//+sqrt(squared)/2);
    //jd.localAnchorB.Set(bodyA->GetPosition().x, bodyA->GetPosition().y);//-sqrt(squared)/2);
    jd.frequencyHz = 20.0f;
    //jd.referenceAngle= 0; //atan2f(diffVec.x, diffVec.y);
    //jd.dampingRatio=1;
    //jd.collideConnected= false;
    //jd.length=5;
    //jd.referenceAngle = 0.0f;
    //jd.motorSpeed = 0.05f * b2_pi;
    //jd.maxMotorTorque = 1e8f;
    //jd.enableMotor = true;
    sltn::getInst().m_world->CreateJoint(&jd);

    ((UserData*)bodyB->GetUserData() )->isConectedToCluster = true;
}

bool Gameplay::TryConnect()
{


    vector<ChosenBody> chosenBodys;
    for (auto& ballA : m_bollekesVec){
        b2Body* bodyA= ballA->GetB2Body(); 
        if( ((UserData*)bodyA->GetUserData() )->isConectedToCluster != true ) continue;

        for (auto& ballB : m_bollekesVec){

            b2Body* bodyB= ballB->GetB2Body(); 
            auto diffVec= (bodyA->GetPosition()-bodyB->GetPosition());
            auto squared= diffVec.LengthSquared();

            //if( ((UserData*)bodyB->GetUserData() )->isConectedToCluster == true ) continue;

            if( squared>50 && squared<30) continue;

            auto chosenBody = ChosenBody( bodyA, bodyB, squared );
            if( chosenBody.numOfJoints >4 ) continue;

            chosenBodys.push_back(chosenBody);

        }
    }
    if( chosenBodys.size()<=0 ) return false;

    // if( chosenBodys.size()<2 ) 
    float minDistance= 9999999.0f;
    ChosenBody cb= ChosenBody();
    for( auto chosenBody : chosenBodys )
    {
        auto distanceSqr= chosenBody.distanceSquared;
        if( distanceSqr < minDistance ){
            minDistance= distanceSqr;
            cb= chosenBody;
        }
    }
    ConnectBodys(cb.bodyA, cb.bodyB);
    return true;
}



void Connect(b2Body* bodyA, b2Body* bodyB){
    if( bodyB == nullptr ) return;

    auto diffVec= (bodyA->GetPosition()-bodyB->GetPosition());
    auto squared= diffVec.LengthSquared();
    if( squared>50) return;

    if( CountJoints(bodyA) >10 ) return;

    ConnectBodys(bodyA,bodyB);
}











void Gameplay::Tick(const float deltaTime)
{
    //if (sf::Keyboard::isKeyPressed(sf::Keyboard::X)){
    //    if( m_player.GetB2Body() ==nullptr ) return;
    //    sltn::getInst().m_world->DestroyJoint(
    //        m_player.GetB2Body()->GetJointList()->joint);
    //    m_player.GetB2Body()->GetJointList()->joint = nullptr;
    //}
    m_mouseTimer+= deltaTime;

    auto worldPos =  sltn::getInst().GetMousePos();

    if( m_mouseTimer>0.05f && sf::Mouse::isButtonPressed(sf::Mouse::Left)){
        m_mouseTimer= 0;

        // - sf::Vector2i( viewrect.x, viewrect.y);
        auto force= to_b2Vec2(worldPos) - b2Vec2(m_player.getPosition().x, m_player.getPosition().y);
        auto len= force.Length();
        force.x /= len;
        force.y /= len;
        //force*=10.0f;

        auto bullet = new Bullet(m_player.getPosition() + to_Vector2( force )
            , atan2(force.y, force.x) );
        force*=55000.0f;
        bullet->GetB2Body()->ApplyForceToCenter( force );
        m_bulletVec.push_front( bullet );
        //m_bulletVec.remove(bullet);
    }

    if( m_mouseTimer>0.5f && sf::Mouse::isButtonPressed(sf::Mouse::Right)){
        m_mouseTimer= 0;

        auto ball = new Ball(worldPos);
        ball->setTexture( sltn::getInst().GetTexture("resources/blue-sphere_512.png") );
        m_bollekesVec.push_back(ball);

        Connect(m_player.GetB2Body(), m_bollekesVec.back()->GetB2Body() );

        for( auto ball1 : m_bollekesVec){

            //if( ( (UserData*)ball1.GetB2Body()->GetUserData() )->isConectedToCluster == false ) continue;
            Connect(ball1->GetB2Body(), m_bollekesVec.back()->GetB2Body() );
        }

        //b2Body* bodyA= m_player.GetB2Body(); // m_playe
        //b2Body* bodyB= m_bollekesVec.back()->GetB2Body(); // ball

    }

    m_player.Tick(deltaTime);

    for (auto& object : m_bollekesVec)
        if( object != nullptr)
            object->Tick(deltaTime);

    for (auto& object : m_bulletVec)
        object->Tick(deltaTime);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
        TryConnect();

    m_View.setCenter(m_player.getPosition());
}

void Gameplay::Paint(sf::RenderWindow& window)
{
    window.setView(m_View);

    //sf::RenderTexture* texture= new sf::RenderTexture(); texture->create(4096, 2048);
    window.draw(backgroundSpr);



    // Debug draw the connections
    unsigned int bCount= sltn::getInst().m_world->GetBodyCount();
    sf::VertexArray vertexArray(sf::Lines, bCount*2);
    for (b2Body* body = sltn::getInst().m_world->GetBodyList(); body; body = body->GetNext()){
        for(b2JointEdge* j =body->GetJointList(); j; j=j->next ){
            vertexArray.append(sf::Vertex(
                sf::Vector2f(j->other->GetPosition().x,j->other->GetPosition().y), sf::Color::Blue ));
            vertexArray.append(sf::Vertex(
                sf::Vector2f(body->GetPosition().x,body->GetPosition().y), sf::Color::Red ));
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




    for (auto& object : m_bollekesVec)
        if( object != nullptr)
            window.draw(*object);


    window.draw(m_player);

    for (auto& object : m_bulletVec)
        window.draw(*object);

	int counter= 0;
	sf::VertexArray spriteVertexArray(sf::PrimitiveType::Quads, m_SpriteAnimationList.size()*4);
	for (auto& object : m_SpriteAnimationList)
		if( object != nullptr){
			//window.draw(*object);
			float size=1.0f;
			spriteVertexArray.append(sf::Vertex(object->getPosition()+sf::Vector2f(-size,-size),sf::Color::Red));
			spriteVertexArray.append(sf::Vertex(object->getPosition()+sf::Vector2f(+size,-size),sf::Color::Red));
			spriteVertexArray.append(sf::Vertex(object->getPosition()+sf::Vector2f(+size,+size),sf::Color::Red));
			spriteVertexArray.append(sf::Vertex(object->getPosition()+sf::Vector2f(-size,+size),sf::Color::Red));
			++counter;
		}

		if ( spriteVertexArray.getVertexCount()>0 ){

			const sf::Texture* tex;
			for (auto& object : m_SpriteAnimationList)
				if( object != nullptr)
					tex= object->getTexture();

			auto it= m_SpriteAnimationList.begin();
			//sf::Transform::Identity
			auto rs= sf::RenderStates(tex); // (*it)->getTexture()
			rs.blendMode= sf::BlendMode::BlendAdd; //,m_View.getTransform, 
			window.draw(spriteVertexArray, rs);

		}

}