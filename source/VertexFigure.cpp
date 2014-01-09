#include "VertexFigure.h"
#include "Player.h"
#include "sltn.h"
#include "Gameplay.h"
#include <Box2D\Box2D.h>
#include <SFML/Graphics/RenderTarget.hpp>

#include <iostream>
#include <fstream>
#include <string>

VertexFigure::VertexFigure(sf::Vector2f pos) : entityBase(), m_va(sf::VertexArray(sf::PrimitiveType::Quads))
{
	b2BodyDef bd;
	bd.type = b2_dynamicBody;// b2_kinematicBody;
	bd.position.Set(pos.x, pos.y);
	if (!bd.position.IsValid()) return;
	bd.awake = false;
	auto ud = new UserData(this);
	ud->kind = UserData::unspecified;
	bd.userData = ud;
	m_b2Body = sltn::getInst().m_world->CreateBody(&bd);

	m_b2Body->SetAngularVelocity(0.5);
	originalPos = m_b2Body->GetPosition();

	// b2PolygonShape shape[4];
	// float size = 20;
	// shape[0].SetAsBox(size, 2, b2Vec2(-size*2.5f, 0), 0);	  m_b2Body->CreateFixture(shape + 0, 2.0f);
	// shape[1].SetAsBox(size, 2, b2Vec2(+size*2.5f, 0), 0);	  m_b2Body->CreateFixture(shape + 1, 2.0f);
	// shape[2].SetAsBox(2, size, b2Vec2(0, -size*2.5f), 0);	  m_b2Body->CreateFixture(shape + 2, 2.0f);
	// shape[3].SetAsBox(2, size, b2Vec2(0, +size*2.5f), 0);	  m_b2Body->CreateFixture(shape + 3, 2.0f);

	setTexture(sltn::getInst().GetTexture("resources/foto.jpg")); //Exp_yellow_maya blue-sphere_512





	// for (auto fixture = m_b2Body->GetFixtureList(); fixture; fixture = fixture->GetNext()){
	// 	auto shape = (b2PolygonShape*)fixture->GetShape();
	// 	auto vec = (shape)->m_vertices;
	// 	for (int i = 0; i < shape->GetVertexCount(); ++i){
	// 		//auto vert= transform.getInverse().transformPoint(sf::Vector2f( vec[i].x,vec[i].y));
	// 		auto vert = sf::Vector2f(vec[i].x, vec[i].y);
	// 		m_va.append(sf::Vertex(vert, sf::Color(50, 80, 90, 200), vert));
	// 	}
	// }
	Load();
}

VertexFigure::~VertexFigure()
{
	if (m_b2Body) sltn::getInst().EnqueDestroyPhysicsEntity(m_b2Body);
	m_b2Body = nullptr;
}


void VertexFigure::Tick(float dt) // 0.0166
{

	if (m_b2Body) // Sync body to sprite
	{
		//m_b2Body->SetAngularVelocity(5);
		//m_b2Body->ApplyForce(b2Vec2(50,50),b2Vec2(0,0));

		auto force = originalPos - m_b2Body->GetPosition();
		force *= 1000.0f;
		m_b2Body->ApplyForceToCenter(force, true);

		auto pos = m_b2Body->GetPosition();
		Sprite::setPosition(pos.x, pos.y);

		float angle = 180.0f / b2_pi*m_b2Body->GetTransform().q.GetAngle();
		Sprite::setRotation(angle);
	}
}


void VertexFigure::setTexture(const sf::Texture& texture, int nrOfFrames)
{
	Sprite::setTexture(texture, false);

	auto rectWidth = (float)getTexture()->getSize().x;
	float size = 9;
	//Sprite::setOrigin( (float)size/2.0f, (float)size/2.0f );
	//Sprite::setScale(size/getTexture()->getSize().x,size/getTexture()->getSize().y);
	Sprite::setTextureRect(sf::IntRect(0, 0, (int)rectWidth, getTexture()->getSize().y));
}

void VertexFigure::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	//states.transform= Vie //sf::Transform::Identity;//this->getTransform();
	states.texture = getTexture(); //;

	//Sprite::draw(target, states);
	//target.draw(*this, states);




	target.draw(m_va, states);


}

using namespace sf;



// Copied and refactored from c#
void VertexFigure::Load()
{
	cout << "\nFile parsing: \n";
	vector<float> readArrPosition;
	vector<float> readArrTexcoord;
	vector<char> readArrColor;

	string line;
	ifstream fs("C:/C_ImportantStuffUnited/_DAE_/3DAE_GD_13/ToolDev/ToolInWpf/ToolInWpf/bin/Debug/saveFile.txt", ios_base::in); // , ios_base::openmode::
	if (fs.is_open())
	{
		// Remove BOM from file begin (0xEFBBBF)
		char a = fs.get(); char b = fs.get(); char c = fs.get();
		if (a != (char)0xEF || b != (char)0xBB || c != (char)0xBF) { fs.seekg(0); }
		else if (a != (char)0xFF || b != (char)0xFE){ fs.seekg(0); }
		else {
			std::cerr << "Warning: file contains the so-called 'UTF-8 signature'\n";
		}
		//char delimeter = '|';
		while (getline(fs, line))
		{
			std::vector<std::string> elements;

			if (line.find("/positions") == 0)
			{
				getline(fs, line);
				split(line, '|', elements);
				for (string s : elements)
					readArrPosition.push_back(std::stof(s.c_str()));

			}
			else if (line.find("/texcoords") == 0)
			{
				getline(fs, line);
				split(line, '|', elements);
				for (string s : elements)
					readArrTexcoord.push_back(std::stof(s.c_str()));
			}
			else if (line.find("/colors") == 0)
			{
				getline(fs, line);
				split(line, '|', elements);
				for (string s : elements)
					readArrColor.push_back(std::stoi(s.c_str()));
			}
			else
			{
				cout << "!non suported line: \n";
			}
		}

		fs.close();
	}
	else std::cerr << "Unable to open file \n";



	if (!(readArrPosition.size() / 2 == readArrTexcoord.size() / 2
		&& (readArrTexcoord.size() / 2 == readArrColor.size() / 4)))
		std::cerr << "Input lengths not valid! \n";




	auto arr = sf::VertexArray(PrimitiveType::Quads, (unsigned int)readArrPosition.size() / 2U);
	try
	{
		for (unsigned int index = 0; index < arr.getVertexCount(); ++index)
		{
			arr[index] = Vertex(Vector2f(readArrPosition[index * 2 + 0], readArrPosition[index * 2 + 1]));
			auto tmp = arr[index];
			tmp.position = sf::Vector2f(readArrPosition[index * 2 + 0], readArrPosition[index * 2 + 1]);
			tmp.texCoords = sf::Vector2f(readArrTexcoord[index * 2 + 0], readArrTexcoord[index * 2 + 1]);
			tmp.color = sf::Color(255, 10, 10, 128);
			tmp.color = Color(readArrColor[index * 4 + 0],
				readArrColor[index * 4 + 1],
				readArrColor[index * 4 + 2],
				readArrColor[index * 4 + 3]);
			arr[index] = tmp;
		}
		m_va = arr;
	}
	catch (exception e)
	{
		std::cerr << "And now loading gives an arror! :p \n";
	}



}