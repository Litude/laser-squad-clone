#ifndef LASER_TEXTFIELD_HPP
#define LASER_TEXTFIELD_HPP

#include <iostream>
#include <stdio.h>
#include <functional>
#include <SFML/Graphics.hpp>
#include <cwctype>

enum tf_type {
  text,
  number
};

class TextField : public sf::Drawable
{
public:
  TextField();
  TextField(int size, sf::RectangleShape Rshape, sf::Font& font, tf_type t = text);
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

  const std::string &getString() const {return t_str;};
  const sf::Vector2f &getPos() const {return t_pos;};
  const sf::Vector2f &getSize() const {return sf::Vector2f(t_width, t_height);};
  size_t getCursorPos() const {return t_index;};
  bool getFocus() const {return t_isFocused;};
  bool getStatus() const {return t_textEntered;};
  const sf::FloatRect getGlobalBounds() const;
  tf_type getType() const {return t_type;};

private:
    virtual void draw(sf::RenderTarget& target,sf::RenderStates states) const;
private:
  tf_type t_type;
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
  unsigned int t_strMaxLength;
};
#endif
