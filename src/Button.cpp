#include "Button.hpp"

Button::Button()
{
}

Button::Button(std::string str, sf::Font font, sf::Uint32 style, int size, sf::Vector2f position)
{
  //Button();
  text.setString(str);
  text.setCharacterSize(size);
  text.setPosition(position);
  text.setFont(font);
  text.setStyle(style);

  text.setFillColor(sf::Color::Black);
  b_hoveredColor = b_color;
  b_color.a = 100;
  b_hoveredColor.a = 255;
}

Button::~Button()
{
}
