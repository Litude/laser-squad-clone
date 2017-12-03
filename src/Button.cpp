#include "Button.hpp"

Button::Button()
{
}

Button::Button(std::string str, sf::Font& font, sf::Uint32 style, int size, sf::Vector2f position)
{
  //Button();
  text.setString(str);
  text.setCharacterSize(size);
  text.setPosition(position);
  text.setFont(font);
  text.setStyle(style);

  text.setFillColor(sf::Color(0, 0, 0, 100));
  b_hoveredColor = sf::Color(0, 0, 0, 255);
  setPosition(position);
  setStyle(style);
  setColor(sf::Color(0, 0, 0, 100));
}

Button::~Button()
{
}

void Button::update(sf::Event& e, sf::RenderWindow& window)
{
  // Mouse handling
  sf::Vector2i mousePos = sf::Mouse::getPosition(window);
  bool isHovering = mousePos.x >= text.getPosition().x && mousePos.x <= text.getPosition().x + text.getLocalBounds().width
                    && mousePos.y >= text.getPosition().y && mousePos.y <= text.getPosition().y + text.getLocalBounds().height;

  switch(b_state) {
    case normal: {
      text.setFillColor(b_color);
      text.setStyle(b_style);
    }
    break;
    case clicked: {
      text.setFillColor(sf::Color::White);
    }
    break;
    case hovered: {
      text.setFillColor(b_hoveredColor);
      text.setStyle(sf::Text::Bold);
    }
    break;
  }

  if(e.type == sf::Event::MouseMoved) {
  if(isHovering){
      b_state = hovered;
    }else{
      b_state = normal;
    }
  }

  if(e.type == sf::Event::MouseButtonPressed) {
    switch(e.mouseButton.button) {
      case sf::Mouse::Left:
      {
        if(isHovering) {
          b_state = clicked;
        }else{
          b_state = normal;
        }
      }
    }
  }
}
