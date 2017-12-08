#ifndef LASER_TEXTFIELD_HPP
#define LASER_TEXTFIELD_HPP

#include <iostream>
#include <stdio.h>
#include <functional>
#include <SFML/Graphics.hpp>

class TextField : public sf::Drawable
{
public:
  TextField();
  TextField(int size, sf::RectangleShape Rshape, sf::Font& font);
  ~TextField();
  void setPosition(sf::Vector2f v);
  void setSize(float w, float h);
  void setString(std::string str);
  void setFont(sf::Font& font) {t_font = font;};
  void setFocus(bool f) {t_isFocused = f;};
  void setRectangleShape(sf::RectangleShape Rshape);
  void setCallback(std::function<void()> callback);
  void update(sf::Event e, sf::RenderWindow& window);
  void click();
  std::string getString() {return t_str;};
  sf::Vector2f getPos() {return t_pos;};
  bool getFocus() {return t_isFocused;};

private:
    virtual void draw(sf::RenderTarget& target,sf::RenderStates states) const;
private:
  bool t_isFocused;
  float t_width;
  float t_height;
  sf::Vector2f t_pos;
  std::string t_str;
  sf::Font t_font;
  sf::RectangleShape t_Rshape;
  sf::Text t_text;
  std::function<void()> t_callback;
};
#endif
