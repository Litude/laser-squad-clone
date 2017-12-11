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
  void setDefaultStr(std::string defS);
  void setFont(sf::Font& font) {t_font = font;};
  void setFocus(bool f) {t_isFocused = f;};
  void setStatus(bool f) {t_textEntered = f;};
  void setRectangleShape(sf::RectangleShape Rshape);
  void setCallback(std::function<void()> callback);
  void setCursor(size_t i);
  void update(sf::Event e, sf::RenderWindow& window);
  void click();

  std::string getString() const {return t_str;};
  sf::Vector2f getPos() const {return t_pos;};
  int getCursorPos() const {return t_index;};
  bool getFocus() const {return t_isFocused;};
  bool getStatus() const {return t_textEntered;};
  const sf::FloatRect  TextField::getGlobalBounds();

private:
    virtual void draw(sf::RenderTarget& target,sf::RenderStates states) const;
private:
  bool t_isFocused;
  bool t_textEntered;
  float t_width;
  float t_height;
  sf::Vector2f t_pos;
  size_t t_index;
  std::string t_str;
  char t_inputChar;
  sf::Font t_font;
  sf::RectangleShape t_Rshape;
  sf::RectangleShape t_Cursor;
  mutable sf::Clock t_clock;
  sf::Text t_text;
  sf::Text t_defText;
  std::function<void()> t_callback;
  int t_strMaxLength;
};
#endif
