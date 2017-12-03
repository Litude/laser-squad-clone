#ifndef LASER_BUTTON_HPP
#define LASER_BUTTON_HPP

#include <iostream>
#include <SFML/Graphics.hpp>
#include "Screen.hpp"

enum state {
   normal = 0,
   clicked = 1,
   hovered = 2,
 };

class Button
{
public:
  Button();
  Button(std::string str,  sf::Font& font, sf::Uint32 style, int size, sf::Vector2f position);
  ~Button();
  void setText(std::string str) {text.setString(str);};
  void setFont(sf::Font& f) {text.setFont(f);};
  void setStyle(sf::Uint32 s) {b_style = s;}
  void setSize(int size) {text.setCharacterSize(size);};
  void setColor(sf::Color c) {b_color = c;};
  void setHovColor(sf::Color hovC) {b_hoveredColor = hovC;}; // Hovered
  void setPosition(sf::Vector2f pos) {b_pos = pos;};
  void setState(sf::Uint32 st) {b_state = st;};
  sf::Text getText() {return text;}
  sf::Vector2f getPos() {return b_pos;}
  sf::Uint32 getState() {return b_state;}
  void update(sf::Event& e, sf::RenderWindow& window);
private:
  sf::Text text;
  sf::Font b_font;
  sf::Color b_color;
  sf::Color b_hoveredColor;
  sf::Vector2f b_pos;
  sf::Uint32 b_state;
  sf::Uint32 b_style;
  float width = text.getLocalBounds().width;
  float height = text.getLocalBounds().height;
};
#endif
