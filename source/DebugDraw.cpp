#include "DebugDraw.h"

MyDebugDraw::MyDebugDraw()
{
    SetFlags( b2Draw::e_shapeBit or b2Draw::e_shapeBit );
}

MyDebugDraw::~MyDebugDraw()
{

}

//convert a Box2D (float 0.0f - 1.0f range) color to a SFML color (uint8 0 - 255 range)
sf::Color MyDebugDraw::B2SFColor(const b2Color &color, int alpha = 255)
{
	sf::Color result((sf::Uint8)(color.r*255), (sf::Uint8)(color.g*255), (sf::Uint8)(color.b*255), (sf::Uint8) alpha);
	return result;
}

void MyDebugDraw::DrawAABB(b2AABB* aabb, const b2Color& color)
{
    sf::ConvexShape convex;
    convex.setOutlineColor(B2SFColor(color));
    convex.setFillColor(B2SFColor(color));
    convex.setPointCount(4);

    convex.setPoint(0, sf::Vector2f(aabb->lowerBound.x, aabb->lowerBound.y));
    convex.setPoint(1, sf::Vector2f(aabb->upperBound.x, aabb->lowerBound.y));
    convex.setPoint(2, sf::Vector2f(aabb->upperBound.x, aabb->upperBound.y));
    convex.setPoint(3, sf::Vector2f(aabb->lowerBound.x, aabb->upperBound.y));

    convex.setOutlineThickness(1.0f);
    this->m_window->draw(convex);
}


void MyDebugDraw::DrawString(int x, int y, const char* string)
{
    // sf::String fpsText;
    // fpsText.setFont(sf::Font::getDefaultFont());
    // fpsText.setSize(15);
    // fpsText.setPosition(x,y);
    // fpsText.setText(string);
    // this->m_window->draw(fpsText);
}

void MyDebugDraw::DrawPoint(const b2Vec2& p, float32 size, const b2Color& color)
{
    std::cout << "MyDebugDraw::DrawPoint is stub\n";
}

void MyDebugDraw::DrawTransform(const b2Transform& xf)
{
    std::cout << "MyDebugDraw::DrawTransform is stub\n";
    // float x,y, lineSize, lineProportion;
    // x = xf.position.x * RATIO;
    // y = xf.position.y * RATIO;
    // lineProportion = 0.15; // 0.15 ~ 10 pixels
    // b2Vec2 p1 = xf.position, p2;
//
	// //red (X axis)
	// p2 = p1 + (lineProportion * xf.R.col1);
    // sf::Shape redLine = sf::Line(p1.x, p1.y, p2.x, p2.y, 1, sf::Color::Red);
//
	// //green (Y axis)
	// p2 = p1 - (lineProportion * xf.R.col2);
	// sf::Shape greenLine = sf::Line(p1.x, p1.y, p2.x, p2.y, 1, sf::Color::Green);
//
    // this->m_window->draw(redLine);
    // this->m_window->draw(greenLine);
}

void MyDebugDraw::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
{
    std::cout << "MyDebugDraw::DrawSegment is stub\n";

    // sf::Shape line = sf::Line(p1.x, p1.y, p2.x, p2.y, 1, this->B2SFColor(color));
    // line.EnableFill(true);
    // this->m_window->draw(line);
}

void MyDebugDraw::DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color)
{
    //no converion in cordinates of center and upper left corner, Circle in sfml is managed by default with the center
    auto circle = sf::CircleShape(radius);
    circle.setPosition(center.x - radius, center.y - radius);
    // line of the circle wich shows the angle
    b2Vec2 p = center + (radius * axis);

    sf::VertexArray lines(sf::Lines, 2);
    lines[0].position = sf::Vector2f(center.x, center.y);
    lines[1].position = sf::Vector2f(p.x, p.y);
    this->m_window->draw(lines);

    this->m_window->draw(circle);
}

void MyDebugDraw::DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color)
{
    auto circle = sf::CircleShape( radius );
    circle.setPosition(center.x - radius, center.y - radius);
    circle.setFillColor(sf::Color::Transparent);
    this->m_window->draw(circle);
}

void MyDebugDraw::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
    sf::ConvexShape convex;
    convex.setOutlineColor(B2SFColor(color));
    convex.setPointCount(vertexCount);

    for (int32 i=0; i<vertexCount; i++)
    {
        convex.setPoint(i, sf::Vector2f(vertices[i].x, vertices[i].y));
    }

    convex.setOutlineThickness(1.0f);
    this->m_window->draw(convex);
}

void MyDebugDraw::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
    sf::VertexArray lines(sf::Lines, vertexCount);

    for (int32 i=0; i<vertexCount; i++)
    {
        lines[i].position = sf::Vector2f(30, 5);
        lines[i].color= B2SFColor(color);
    }

    this->m_window->draw(lines);
}


void MyDebugDraw::DrawMouseJoint(b2Vec2& p1, b2Vec2& p2, const b2Color &boxColor, const b2Color &lineColor)
{
    std::cout << "MyDebugDraw::DrawMouseJoint is stub";

    // sf::Shape polygon;
    // sf::Shape polygon2;
    // float p1x = p1.x * RATIO;
    // float p1y = p1.y * RATIO;
    // float p2x = p2.x * RATIO;
    // float p2y = p2.y * RATIO;
    // float size = 4.0f;
    //
    // sf::Color boxClr = this->B2SFColor(boxColor);
    // sf::Color lineClr = this->B2SFColor(lineColor);
    //
    // //first green box for the joint
    // polygon.AddPoint(p1x-size/2, p1y-size/2, boxClr);
    // polygon.AddPoint(p1x+size/2, p1y-size/2, boxClr);
    // polygon.AddPoint(p1x+size/2, p1y+size/2, boxClr);
    // polygon.AddPoint(p1x-size/2, p1y+size/2, boxClr);
    //
    // //second green box for the joint
    // polygon2.AddPoint(p2x-size/2, p2y-size/2, boxClr);
    // polygon2.AddPoint(p2x+size/2, p2y-size/2, boxClr);
    // polygon2.AddPoint(p2x+size/2, p2y+size/2, boxClr);
    // polygon2.AddPoint(p2x-size/2, p2y+size/2, boxClr);
    //
    // sf::Shape line = sf::Line(p1x, p1y, p2x, p2y, 1, lineClr);
    // line.EnableFill(true);
    //
    // this->m_window->draw(polygon);
    // this->m_window->draw(polygon2);
    // this->m_window->draw(line);
}
