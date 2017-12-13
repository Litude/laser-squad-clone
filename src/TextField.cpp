#include "TextField.hpp"
#include "constants.hpp"

TextField::TextField()
{
}

TextField::TextField(int size, sf::RectangleShape Rshape, sf::Font& font, tf_type t)
{
  t_text.setTextColor(sf::Color::Black);
  t_text.setCharacterSize(size);
  t_text.setFont(font);

  t_defText.setTextColor(sf::Color::Black);
  t_defText.setCharacterSize(size);
  t_defText.setFont(font);


  t_Cursor.setFillColor(sf::Color::Black);
  t_Cursor.setSize(sf::Vector2f(2.f, 30.f));

  setCursor(0);

  t_strMaxLength = 10;
  setFocus(false);
  setStatus(false);
  t_Rshape = Rshape;

  t_Rshape.setFillColor(sf::Color::White);
  t_Rshape.setOutlineColor(sf::Color::Black);

  t_type = t;
}

TextField::~TextField()
{
}

void TextField::setSize(float w, float h)
{
  t_width = w;
  t_height = h;
  t_Rshape.setSize(sf::Vector2f(t_width, t_height));
}

void TextField::setRectangleShape(sf::RectangleShape Rshape)
{
  t_Rshape = Rshape;
  setPosition(getPos());
}

void TextField::setPosition(sf::Vector2f v)
{
  t_pos = v;

  t_Rshape.setOrigin(t_Rshape.getGlobalBounds().width/2, t_Rshape.getGlobalBounds().height/2);
  t_Rshape.setPosition(t_pos);

  sf::Vector2f textPosition = sf::Vector2f(t_Rshape.getPosition().x, t_Rshape.getPosition().y + t_Rshape.getGlobalBounds().height / 8);
  t_text.setOrigin(t_Rshape.getGlobalBounds().width / 2.3f, t_Rshape.getGlobalBounds().height / 2);
  t_text.setPosition(textPosition);

  sf::Vector2f defTextPosition = sf::Vector2f(t_Rshape.getPosition().x, t_Rshape.getPosition().y - t_Rshape.getGlobalBounds().height / 8);
  t_defText.setOrigin(t_defText.getGlobalBounds().width / 2, t_defText.getGlobalBounds().height / 2);
  t_defText.setPosition(defTextPosition);

  t_Cursor.setOrigin(t_text.getOrigin());
}

void TextField::setString(std::string s)
{
  t_str = s;
  t_text.setString(t_str);
}

void TextField::setDefaultStr(std:: string defS)
{
  t_defText.setString(defS);
}

void TextField::update(sf::Event e, sf::RenderWindow& window)
{
  sf::Vector2i mousePos = sf::Mouse::getPosition(window);

	sf::Vector2f objPos;
	sf::FloatRect objGlobalBounds;
  objPos = t_Rshape.getPosition();
  objGlobalBounds = t_Rshape.getGlobalBounds();

  bool isHovering = mousePos.x >= objPos.x - objGlobalBounds.width / 2
		             && mousePos.x <= objPos.x + objGlobalBounds.width / 2
		            && mousePos.y >= objPos.y - objGlobalBounds.height / 2
		            && mousePos.y <= objPos.y + objGlobalBounds.height / 2;

  if (e.type == sf::Event::MouseButtonPressed){
    switch (e.mouseButton.button) {
      case sf::Mouse::Left: {
        if (isHovering) {
          setFocus(true);
        }else{
          setFocus(false);
        }
      }
        break;
      default:
        break;
      }
    }

  if(getFocus()) {
    if(e.type == sf::Event::TextEntered) {
      if(e.text.unicode == 8){ // Backspace pressed
        if(t_index > 0){
          --t_index;
          t_str = t_str.erase(t_index , 1);
        }
      }else if(e.text.unicode == 27){ // Escape pressed
        t_str.clear();
        t_text.setString(t_str);
        t_index = 0;
        setStatus(false);
        setFocus(false);
      }else if(e.text.unicode == '\r'){ // Return pressed
        if(!t_text.getString().isEmpty()){ // If an empty string is returned the default text will be displayed
          setStatus(true);
        }else{
          setStatus(false);
        }
        setFocus(false);
      }else{
        if(getType() == number) {
          if(!std::iswdigit(e.text.unicode)) {
            return;
          }
        }
        if(t_str.length() < t_strMaxLength){  // Writing text
          t_inputChar = static_cast<char>(e.text.unicode);
          t_str.insert(t_index, 1, t_inputChar);
          ++t_index;
        }
      }
      t_text.setString(t_str);
    }else if(e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Left && t_index > 0){ // Left arrow
      --t_index;
    }else if(e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Right && t_index < t_str.size()){ // Right arrow
      ++t_index;
    }
    if(t_text.getString().isEmpty()) {
      setStatus(false);
    }else{
      setStatus(true);
    }
    setCursor(t_index);
  }
}

const sf::FloatRect TextField::getGlobalBounds() const
{
  return t_Rshape.getGlobalBounds();
}

void TextField::draw(sf::RenderTarget& target,sf::RenderStates states) const
{
  target.draw(t_Rshape, states);
  if(getFocus()){ // Text field has focus
    target.draw(t_text, states);

    float blink = t_clock.getElapsedTime().asSeconds();
		if (blink >= 0.8f) {t_clock.restart();}
    if (blink < 0.4f){
      target.draw(t_Cursor, states);
    }
  }else if(getStatus()){  // User has pressed return
    target.draw(t_text, states);
  }else{  // Display default text
    target.draw(t_defText, states);
  }
}

void TextField::setCallback(std::function<void()> callback)
{
	this->t_callback = callback;
}

void TextField::click()
{
	if (t_callback != nullptr) {
		t_callback();
	}
}

void TextField::setCursor(size_t i) // Updates the position of the cursor
{
  if(i <= t_str.size())
  {
    t_index = i;
    t_Cursor.setPosition(t_text.findCharacterPos(t_index).x + t_Rshape.getGlobalBounds().width / 2.3f, t_text.findCharacterPos(t_index).y + t_Rshape.getGlobalBounds().height / 2);
  }
}
