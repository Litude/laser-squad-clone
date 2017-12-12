#ifndef LASER_BUTTON_HPP
#define LASER_BUTTON_HPP

#include <iostream>
#include <SFML/Graphics.hpp>
#include <functional>
#include "Screen.hpp"

class Button : public sf::Drawable
{
public:
  Button();
  Button(std::string str,  sf::Font& font, sf::Uint32 style, int size, sf::Vector2f position);
  Button(std::string str,  sf::Font& font, sf::Uint32 style, int size, sf::Vector2f position, sf::Sprite sprite);
  Button(std::string str,  sf::Font& font, sf::Uint32 style, int size, sf::Vector2f position, sf::RectangleShape Rshape);
  ~Button();

  enum state {
	  normal = 0,
	  clicked = 1,
	  hovered = 2,
  };

  enum button_type {
	  text = 0,
	  sprite = 1,
	  rectangle = 2
  };

  void setText(std::string s);
  void setFont(sf::Font& f) {b_font = f;};
  void setStyle(sf::Uint32 s) {b_style = s;}
  void setSize(int size) {b_text.setCharacterSize(size);};
  void setColor(sf::Color c) {b_color = c;};
  void setHovColor(sf::Color hovC) {b_hoveredColor = hovC;};
  void setPosition(sf::Vector2f pos);
  void setState(sf::Uint32 st);
  void setSprite(sf::Sprite s) {b_sprite = s;};
  void setRectangleShape(sf::RectangleShape Rshape);

  const sf::Text& getText() const {return b_text;}
  const sf::Vector2f& getPos() const {return b_pos;}
  const sf::Uint32& getState() const {return b_state;}
  const sf::FloatRect getGlobalBounds() const;
  void update(sf::Event& e, sf::RenderWindow& window);
  void setCallback(std::function<void()> callback);
  void click();
private:
    virtual void draw(sf::RenderTarget& target,sf::RenderStates states) const;
private:
  sf::Text b_text;
  std::string str;
  sf::Font b_font;
  sf::Color b_color;
  sf::Color b_hoveredColor;
  sf::Vector2f b_pos;
  sf::Uint32 b_state;
  sf::Uint32 b_style;
  button_type b_type;
  float b_width = b_text.getLocalBounds().width;
  float b_height = b_text.getLocalBounds().height;
  std::function<void()> b_callback;
  sf::Sprite b_sprite;
  sf::RectangleShape b_Rshape;
};
#endif
