#include "Button.hpp"

Button::Button()
{
}

// text-only button
Button::Button(std::string str, sf::Font& font, sf::Uint32 style, int size, sf::Vector2f position)
{
  //Button();
  b_type = text;
  b_text.setString(str);
  b_text.setCharacterSize(size);
  //b_text.setPosition(position);
  b_text.setFont(font);
  b_text.setStyle(style);
  b_text.setOrigin(b_text.getGlobalBounds().width / 2, b_text.getGlobalBounds().height / 2);

  b_text.setFillColor(sf::Color(0, 0, 0, 100));
  b_hoveredColor = sf::Color(0, 0, 0, 255);
  setPosition(position);
  setStyle(style);
  setColor(sf::Color(0, 0, 0, 100));
}

// Button with sprite
Button::Button(std::string str, sf::Font& font, sf::Uint32 style, int size, sf::Vector2f position, sf::Sprite spr)
{
  //Button();
  b_type = sprite;
  b_text.setString(str);
  b_text.setCharacterSize(size);
  b_text.setPosition(position);
  b_text.setFont(font);
  b_text.setStyle(style);
  b_text.setOrigin(b_text.getGlobalBounds().width / 2, b_text.getGlobalBounds().height / 2);

  b_text.setFillColor(sf::Color(0, 0, 0, 100));
  b_hoveredColor = sf::Color(0, 0, 0, 255);
  setPosition(position);
  setStyle(style);
  setColor(sf::Color(0, 0, 0, 100));

  setSprite(spr);
  setText(str);
}

// Button with rectangle shape
Button::Button(std::string str, sf::Font& font, sf::Uint32 style, int size, sf::Vector2f position, sf::RectangleShape Rshape)
{
  //Button();
  b_type = rectangle;
  setRectangleShape(Rshape);
  setText(str);
  b_text.setString(str);
  b_text.setCharacterSize(size);
  b_text.setPosition(position);
  b_text.setFont(font);
  b_text.setStyle(style);
  b_text.setOrigin(b_text.getGlobalBounds().width / 2, b_text.getGlobalBounds().height / 2);

  b_text.setFillColor(sf::Color(0, 0, 0, 100));
  b_hoveredColor = sf::Color(0, 0, 0, 255);
  setPosition(position);
  setStyle(style);
  setColor(sf::Color(0, 0, 0, 100));


}

Button::~Button()
{
}

void Button::setRectangleShape(sf::RectangleShape Rshape) 
{
	b_Rshape = Rshape;
	setPosition(getPos());
};

const sf::FloatRect  Button::getGlobalBounds()
{
	switch (b_type) {
	case text:
	{
		return b_text.getGlobalBounds();
	}
	break;
	case sprite:
	{
		return b_sprite.getGlobalBounds();
	}
	break;
	case rectangle:
	{
		return b_Rshape.getGlobalBounds();
	}
	break;
	}
}

void Button::setState(sf::Uint32 st)
{
  b_state = st;
  switch(b_state) {
    case normal: {
      b_text.setFillColor(b_color);
      b_text.setStyle(b_style);
    }
    break;
    case clicked: {
      b_text.setFillColor(sf::Color::White);
    }
    break;
    case hovered: {
      b_text.setFillColor(b_hoveredColor);
      b_text.setStyle(sf::Text::Bold);
    }
    break;
  }
}

void Button::setPosition(sf::Vector2f pos)
{
  b_pos = pos;
  switch(b_type) {
    case text: {
		b_text.setOrigin(b_text.getGlobalBounds().width / 2, b_text.getGlobalBounds().height / 2);
      b_text.setPosition(b_pos);
    }
    break;
    case sprite: {
      b_sprite.setOrigin(b_sprite.getGlobalBounds().width/2, b_sprite.getGlobalBounds().height/2);
      b_sprite.setPosition(b_pos);
      sf::Vector2f textPosition = sf::Vector2f(b_sprite.getPosition().x, b_sprite.getPosition().y);
      b_text.setOrigin(b_text.getGlobalBounds().width/2, b_text.getGlobalBounds().height/2);
      b_text.setPosition(textPosition);
    }
    break;
    case rectangle: {
      b_Rshape.setOrigin(b_Rshape.getGlobalBounds().width/2, b_Rshape.getGlobalBounds().height/2);
      b_Rshape.setPosition(b_pos);
	  sf::Vector2f textPosition = sf::Vector2f(b_Rshape.getPosition().x, b_Rshape.getPosition().y - b_Rshape.getGlobalBounds().height / 8);
      b_text.setOrigin(b_text.getGlobalBounds().width/2, b_text.getGlobalBounds().height/2);
      b_text.setPosition(textPosition);
    }
    break;
  }



}

void Button::update(sf::Event& e, sf::RenderWindow& window)
{
  // Mouse handling
  sf::Vector2i mousePos = sf::Mouse::getPosition(window);

  sf::Vector2f objPos;
  sf::FloatRect objGlobalBounds;

  switch(b_type){
    case text: {
      objPos = b_text.getPosition();
      objGlobalBounds = b_text.getGlobalBounds();
	  objPos.y += b_text.getGlobalBounds().height * 0.5f;
    }
    break;
    case sprite: {
	  objPos = b_sprite.getPosition();
	  objGlobalBounds = b_sprite.getGlobalBounds();
    }
    break;
    case rectangle: {
	  objPos = b_Rshape.getPosition();
	  objGlobalBounds = b_Rshape.getGlobalBounds();
    }
    break;
  }
  /*
  bool isHovering = mousePos.x >= b_text.getPosition().x && mousePos.x <= b_text.getPosition().x + b_text.getLocalBounds().width
                  && mousePos.y >= b_text.getPosition().y && mousePos.y <= b_text.getPosition().y + b_text.getLocalBounds().height + 10.f;
	*/

  bool isHovering =    mousePos.x >= objPos.x - objGlobalBounds.width / 2
                       && mousePos.x <= objPos.x + objGlobalBounds.width / 2
                       && mousePos.y >= objPos.y - objGlobalBounds.height / 2
                       && mousePos.y <= objPos.y + objGlobalBounds.height / 2;

                    /*   bool mouseInButton =    m_mousePosition.x >= m_button.getPosition().x - m_button.getGlobalBounds().width/2
                            && m_mousePosition.x <= m_button.getPosition().x + m_button.getGlobalBounds().width/2
                            && m_mousePosition.y >= m_button.getPosition().y - m_button.getGlobalBounds().height/2
                            && m_mousePosition.y <= m_button.getPosition().y + m_button.getGlobalBounds().height/2;
*/
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
		  click();
        }else{
          b_state = normal;
        }
      }
      break;
    }
  }
}

void Button::draw(sf::RenderTarget& target,sf::RenderStates states) const
{
  if(b_type == sprite) {
    target.draw(b_sprite, states);
  }
  if(b_type == rectangle) {
    target.draw(b_Rshape, states);
  }
  target.draw(b_text, states);
}

void Button::setCallback(std::function<void()> callback)
{
	this->b_callback = callback;
}

void Button::click()
{
	if (b_callback != nullptr) {
		b_callback();
	}
}
