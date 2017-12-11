#include "Line.hpp"

sf::Line::Line() :
	point1(sf::Vector2f(0.f, 0.f)), point2(sf::Vector2f(0.f, 0.f)), color1(sf::Color::Yellow), color2(sf::Color::Blue), thickness(3.f)
{
}

sf::Line::Line(const sf::Vector2f& point1, const sf::Vector2f& point2) :
	color1(sf::Color::Yellow), color2(sf::Color::Blue), thickness(3.f)
{
	this->point1 = point1;
	this->point2 = point2;
	calculateVertices();
}

void sf::Line::setPositionPoint1(const sf::Vector2f& point)
{
	this->point1 = point;
	calculateVertices();
}

void sf::Line::setPositionPoint2(const sf::Vector2f& point)
{
	this->point2 = point;
	calculateVertices();
}

void sf::Line::setColorPoint1(const sf::Color& color)
{
	this->color1 = color;
	calculateVertices();
}

void sf::Line::setColorPoint2(const sf::Color& color)
{
	this->color2 = color;
	calculateVertices();
}

void sf::Line::calculateVertices()
{
	sf::Vector2f direction = point2 - point1;
	sf::Vector2f unitDirection = direction / std::sqrt(direction.x * direction.x + direction.y * direction.y);
	sf::Vector2f unitPerpendicular(-unitDirection.y, unitDirection.x);

	sf::Vector2f offset = (thickness / 2.f) * unitPerpendicular;

	vertices[0].position = point1 + offset;
	vertices[1].position = point2 + offset;
	vertices[2].position = point2 - offset;
	vertices[3].position = point1 - offset;

	vertices[0].color = color1;
	vertices[1].color = color2;
	vertices[2].color = color2;
	vertices[3].color = color1;
}

void sf::Line::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	target.draw(vertices, 4, sf::Quads);
}