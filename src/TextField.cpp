#include "TextField.hpp"

TextField::TextField()
{
}

TextField::TextField(int size, sf::RectangleShape Rshape, sf::Font& font)
{
  t_text.setFillColor(sf::Color::Black);
  t_text.setCharacterSize(size);
  t_text.setFont(font);
  setFocus(false);
  t_Rshape = Rshape;
  t_Rshape.setFillColor(sf::Color::White);
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
  sf::Vector2f textPosition = sf::Vector2f(t_pos.x, t_pos.y);
  t_text.setOrigin(t_Rshape.getLocalBounds().width / 2 * 0.9, t_Rshape.getLocalBounds().height / 2);
  t_text.setPosition(textPosition);
}

void TextField::setString(std::string s)
{
  t_str = s;
  t_text.setString(t_str);
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
      switch (e.key.code)
      {
        case sf::Keyboard::BackSpace: // NOT WORKING
          t_str = t_str.substr(0, t_str.length() - 1);
          break;
        case sf::Keyboard::Escape:
          t_isFocused = false;
          break;
        case sf::Keyboard::Return:
          break;
        default:
          t_str += static_cast<char>(e.text.unicode);
          break;
      }
      t_text.setString(t_str);
    }
  }

}

void TextField::draw(sf::RenderTarget& target,sf::RenderStates states) const
{
  target.draw(t_Rshape, states);
  target.draw(t_text, states);
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
