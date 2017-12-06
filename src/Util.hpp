#ifndef UTIL_H
#define UTIL_H

#include <vector>
#include <SFML/System/Vector2.hpp>
#include <stdlib.h>

namespace Util {

	inline const std::vector<sf::Vector2u> traceLine(sf::Vector2i origin, sf::Vector2i target) {
		std::vector<sf::Vector2u> pierced;
		int dx = abs(target.x - origin.x);
		int dy = abs(target.y - origin.y);
		int x = origin.x;
		int y = origin.y;
		int n = 1 + dx + dy;
		int x_inc = (target.x > origin.x) ? 1 : -1;
		int y_inc = (target.y > origin.y) ? 1 : -1;
		int error = dx - dy;
		dx *= 2;
		dy *= 2;

	    for (; n > 0; --n) {
			//std::cout << "adding coord: (" << x << ", " << y << ")" << std::endl;
			unsigned add_x = (x < 0 ? 0 : x);
			unsigned add_y = (y < 0 ? 0 : y);
			pierced.push_back(sf::Vector2u(add_x, add_y));

			if (error > 0) {
				x += x_inc;
				error -= dy;
			} else {
				y += y_inc;
				error += dx;
	        }
	    }
	    return pierced;
	}

	template <typename T>
	const sf::Vector2f mapToPixels(sf::Vector2<T> coords) {
		return sf::Vector2f((coords.x * TILESIZE) + TILESIZE/2, (coords.y * TILESIZE) + TILESIZE/2);
	}
	
	/*inline const std::vector<sf::Vector2u> traceLine(sf::Vector2i origin, sf::Vector2i target) {
		//return all square coordinates pierced by line
		std::vector<sf::Vector2u> pierced;
		int x_sign = (target.x > origin.x) ? 1 : -1;
		int y_sign = (target.y > origin.y) ? 1 : -1;

		float slope = ((float) target.y - origin.y) / (target.x - origin.x);

		for (int x = 0; x <= abs(target.x - origin.x); x += x_sign) {//+= x_sign) {
			for (int y = 0; y <= abs(target.y - origin.y); y += y_sign) {// += y_sign) {
				float xf = (float) y / slope;
				if ( ((int) (xf + (x_sign * 0.5))) == x ) {
					std::cout << "adding coord: (" << x + origin.x << ", " << y + origin.y << ")" << std::endl; 
					sf::Vector2u coord( (unsigned) x + origin.x, (unsigned) y + origin.y);
					pierced.push_back(coord);
				}
			}
		}
		return pierced;
	}*/

}

#endif