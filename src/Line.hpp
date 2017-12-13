#ifndef SF_LINE
#define SF_LINE

#include <SFML/Graphics.hpp>
#include <cmath>

namespace sf
{
	class Line : public sf::Drawable, public sf::Transformable
	{
	public:
		Line();
		Line(const sf::Vector2f& point1, const sf::Vector2f& point2);
		void setPositionPoint1(const sf::Vector2f& point);
		void setPositionPoint2(const sf::Vector2f& point);
		void setColorPoint1(const sf::Color& color);
		void setColorPoint2(const sf::Color& color);
		const sf::Color& getColorPoint1() { return color1; }
		const sf::Color& getColorPoint2() { return color2; }
		const sf::Vector2f& getPositionPoint1() { return point1; }
		const sf::Vector2f& getPositionPoint2() { return point2; }
		void draw(sf::RenderTarget &target, sf::RenderStates states) const;
	private:
		sf::Color color1;
		sf::Color color2;
		float thickness;		
		sf::Vector2f point1;
		sf::Vector2f point2;
		sf::Vertex vertices[4];
		void calculateVertices();
	};
}

#endif
