#include "config.h"
#include <iostream>
#include <algorithm>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include "Game.hpp"
#include "Map.hpp"
#include "constants.hpp"

int main(int argc, char* argv[]) {
	
  sf::RenderWindow App(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Laser Squad");

  //Create stationary view
  sf::View fixedView = App.getView();

  //Gameport view
  sf::View gameView(sf::FloatRect(0, 0, WINDOW_WIDTH - MENUSIZE, WINDOW_HEIGHT));
  gameView.setSize(WINDOW_WIDTH - MENUSIZE, WINDOW_HEIGHT);
  gameView.setCenter((WINDOW_WIDTH - MENUSIZE) / 2, WINDOW_HEIGHT/2);
  gameView.setViewport(sf::FloatRect(0, 0, static_cast<float>(WINDOW_WIDTH - MENUSIZE)/WINDOW_WIDTH, 1));

  sf::RectangleShape selectedCharacter(sf::Vector2f(TILESIZE, TILESIZE));
  selectedCharacter.setOutlineColor(sf::Color::Yellow);
  selectedCharacter.setOutlineThickness(2.0f);
  selectedCharacter.setFillColor(sf::Color::Transparent);


  sf::Texture texPlayer1;
  if (!texPlayer1.loadFromFile("img/character1_sheet.png")) {
	  std::cout << "Could not load 'img/character1_sheet.png'\n";
  }
  sf::Texture texPlayer2;
  if (!texPlayer2.loadFromFile("img/character2_sheet.png")) {
	  std::cout << "Could not load 'img/character2_sheet.png'\n";
  }

  //Initialize font
  sf::Font font;
  if (!font.loadFromFile("font/ARIALN.TTF")) {
	std::cout << "Could not load 'font/ARIALN.TTF'\n";
  }
  std::vector<sf::Sprite> characterShapes;
  std::vector<sf::Sprite> mapTiles;

  //Initialize fps clock
  sf::Clock fpsclock;
  float lastTime = 0;
  float currentTime = 0;

  //Initialize delta-clock
  sf::Clock clock;
  double timeStep = 16000;
  double timeAccumulator = 0;

  //Interface
  sf::RectangleShape interfaceBkg(sf::Vector2f(MENUSIZE, App.getSize().y));
  interfaceBkg.setPosition(App.getSize().x - MENUSIZE,0);
  interfaceBkg.setFillColor(sf::Color::Blue);

  //End turn button
  sf::RectangleShape button(sf::Vector2f(100, 80));
  button.setFillColor(sf::Color::Magenta);
  button.setPosition(650, 260);
  sf::Text textEndTurn;
  textEndTurn.setFont(font);
  textEndTurn.setString("End turn");
  textEndTurn.setPosition(652, 280);

  //FPS counter text
  sf::Text textFPS;
  textFPS.setFont(font);
  textFPS.setCharacterSize(12);
  textFPS.setPosition(652, 0);

  //Current turn text
  sf::Text textCurTurn;
  textCurTurn.setFont(font);
  textCurTurn.setCharacterSize(12);
  textCurTurn.setPosition(652, 50);
  
  Game game;

  game.initializeMap(30, 30);
  game.getMap().getTile(12, 12).setTile("img/tile_pavement.png", true); //Add one solid block for collision testing

  game.addCharacter(sf::Vector2u(0, 0), 1);
  game.addCharacter(sf::Vector2u(4, 4), 1);
  game.addCharacter(sf::Vector2u(8, 8), 2);
  game.addCharacter(sf::Vector2u(10, 10), 2);

  //The size of the character or map vector should not change after initialization
  characterShapes.resize(game.getMap().getCharacters().size());
  mapTiles.resize(game.getMap().getTileMap().size()*game.getMap().getTileMap().at(0).size());

  for (unsigned int i = 0; i < game.getMap().getTileMap().size(); ++i) {
	  for (unsigned int j = 0; j < game.getMap().getTileMap()[i].size(); j++) {
		  mapTiles[i*game.getMap().getTileMap().size() + j].setPosition(j * TILESIZE, i * TILESIZE);
		  mapTiles[i*game.getMap().getTileMap().size() + j].setTexture(game.getMap().getTile(j, i).getTexture());
	  }
  }

  while (App.isOpen()) {
	textCurTurn.setString("Current turn: Player " + std::to_string(game.getCurrentPlayer()));
    sf::Event Event;
    while (App.pollEvent(Event)) {
      if (Event.type == sf::Event::Closed) {
		  App.close();
	  }
	  //Handle keyboard input
	  if (Event.type == sf::Event::KeyPressed) {
		  switch (Event.key.code) {
		  case sf::Keyboard::Left:
			  if (game.getSelectedCharacter() != -1 && game.getMap().getCharacters()[game.getSelectedCharacter()].isMoving() == false) {
				  game.getMap().characterMoveLeft(game.getSelectedCharacter());
			  }
			  break;
		  case sf::Keyboard::Right:
			  if (game.getSelectedCharacter() != -1 && game.getMap().getCharacters()[game.getSelectedCharacter()].isMoving() == false) {
				  game.getMap().characterMoveRight(game.getSelectedCharacter());
			  }
			  break;
		  case sf::Keyboard::Down:
			  if (game.getSelectedCharacter() != -1 && game.getMap().getCharacters()[game.getSelectedCharacter()].isMoving() == false) {
				  game.getMap().characterMoveDown(game.getSelectedCharacter());
			  }
			  break;
		  case sf::Keyboard::Up:
			  if (game.getSelectedCharacter() != -1 && game.getMap().getCharacters()[game.getSelectedCharacter()].isMoving() == false) {
				  game.getMap().characterMoveUp(game.getSelectedCharacter());
			  }
			  break;

		  case sf::Keyboard::A:
			  if (gameView.getCenter().x - (App.getSize().x - MENUSIZE) / 2 > 0) gameView.move(-TILESIZE, 0);
			  break;
		  case sf::Keyboard::D:
			  if (gameView.getCenter().x + (App.getSize().x - MENUSIZE) / 2 < game.getMap().getSizeX() * TILESIZE) gameView.move(TILESIZE, 0);
			  break;
		  case sf::Keyboard::S:
			  if (gameView.getCenter().y + App.getSize().y / 2 < game.getMap().getSizeY() * TILESIZE) gameView.move(0, TILESIZE);
			  break;
		  case sf::Keyboard::W:
			  if (gameView.getCenter().y - App.getSize().y / 2 > 0) gameView.move(0, -TILESIZE);
			  break;
		  }
		  //std::cout << Event.key.code << std::endl;
	  }
	  //Handle mouse input
	if (Event.type == sf::Event::MouseButtonPressed) {
		//std::cout << Event.mouseButton.button << " " << Event.mouseButton.x << " " << Event.mouseButton.y << std::endl;

		if (Event.mouseButton.x >= App.getSize().x - MENUSIZE) {
			//std::cout << "Clicked on the menubar" << std::endl;
		} else {
			//std::cout << "Clicked on the gamescreen" << std::endl;

			//std::cout << App.mapPixelToCoords(sf::Mouse::getPosition(App), gameView).x << " " << App.mapPixelToCoords(sf::Mouse::getPosition(App), gameView).y << std::endl;
			
			// Click x and y in map tile coordinates
			unsigned int xCoord = App.mapPixelToCoords(sf::Mouse::getPosition(App), gameView).x / TILESIZE;
			unsigned int yCoord = App.mapPixelToCoords(sf::Mouse::getPosition(App), gameView).y / TILESIZE;

			//std::cout << xCoord - (xCoord % TILESIZE) << " " << yCoord - (yCoord % TILESIZE) << std::endl;
			for (unsigned int i = 0; i < game.getMap().getCharacters().size(); ++i) {
				if (game.getMap().getCharacters()[i].getTeam() == game.getCurrentPlayer() &&
					game.getMap().getCharacters()[i].getPosition().x <= xCoord &&
					game.getMap().getCharacters()[i].getPosition().x + 1 >= xCoord &&
					game.getMap().getCharacters()[i].getPosition().y <= yCoord &&
					game.getMap().getCharacters()[i].getPosition().y + 1 >= yCoord) {
					game.setSelectedCharacter(i); //Select clicked character
					break;
				} else {
					game.setSelectedCharacter(-1);
				}
			}

		}


		  if (Event.mouseButton.button == 0) {
			if (Event.mouseButton.x >= App.getSize().x - MENUSIZE + 50 && Event.mouseButton.y >= 260 && Event.mouseButton.x <= App.getSize().x - 50 && Event.mouseButton.y <= 340) {
				game.endTurn();
				std::cout << "Pressed the end turn button" << std::endl;
			}
		  }
	  }
	  if (Event.type == sf::Event::Resized) {
		  //Update fixed interface

			  //Update view
			  fixedView.setSize(Event.size.width, Event.size.height);
			  fixedView.setCenter(Event.size.width/2, Event.size.height/2);
			  std::cout << fixedView.getCenter().x << " " << fixedView.getCenter().y << std::endl;

			  //Update background
			  interfaceBkg.setPosition(Event.size.width - MENUSIZE, 0);
			  interfaceBkg.setSize(sf::Vector2f(MENUSIZE, Event.size.height));
			  std::cout << Event.size.width - MENUSIZE << std::endl;

			  //Update FSP text
			  textFPS.setPosition(Event.size.width - MENUSIZE + 52, 0);

			  //Update turn text
			  textCurTurn.setPosition(Event.size.width - MENUSIZE + 52, 50);

			  //Update end turn button
			  button.setPosition(Event.size.width - MENUSIZE + 50, 260);
			  textEndTurn.setPosition(Event.size.width - MENUSIZE + 52, 280);

		//Update game view
			  gameView.setSize(Event.size.width - MENUSIZE, Event.size.height);
			  gameView.setCenter((Event.size.width - MENUSIZE) / 2, Event.size.height / 2); //TODO: This needs to take into account changes to the view
			  gameView.setViewport(sf::FloatRect(0, 0, static_cast<float>(Event.size.width - MENUSIZE) / Event.size.width, 1));
			  std::cout << gameView.getViewport().width << std::endl;

	  }
    }

	currentTime = fpsclock.getElapsedTime().asMicroseconds() / 1000000.0f;
	float fps = 1.f / (currentTime - lastTime);
	textFPS.setString("FPS: " + std::to_string(fps));
	lastTime = currentTime;

	//Handle time delta dependent actions
	double delta = clock.restart().asMicroseconds();
	timeAccumulator += delta;

	while(timeAccumulator > timeStep) {
		timeAccumulator -= timeStep;

		for (auto &character : game.getMap().getCharacters()) {
			if (character.isMoving()) {
				character.move(timeStep / 1000.0f);
			}
		}

	}

	App.clear();

	//Draw UI
	App.setView(fixedView);
	App.draw(interfaceBkg); //Must always be first since it covers the whole area and will hide anything "under" it
	App.draw(textFPS);
	App.draw(button);
	App.draw(textCurTurn);
	App.draw(textEndTurn);


	//Draw game
	App.setView(gameView);

	//Draw map

	//Calculate which tiles we can actually see before drawing the map
	int upperamount = std::max(static_cast<int>((gameView.getCenter().y - App.getSize().y/2) / TILESIZE), 0);
	int leftamount = std::max(static_cast<int>((gameView.getCenter().x - (App.getSize().x - MENUSIZE)/2) / TILESIZE), 0);
	int downamount = std::min(static_cast<int>(upperamount + App.getSize().y / TILESIZE) , static_cast<int>(game.getMap().getSizeY() - 1));
	int rightamount = std::min(static_cast<int>(leftamount + (App.getSize().x - MENUSIZE) / TILESIZE), static_cast<int>(game.getMap().getSizeX() - 1));

	for (unsigned int yi = upperamount; yi <= downamount; ++yi) {
		for (unsigned int xi = leftamount; xi <= rightamount; xi++) {
			App.draw(mapTiles[yi*game.getMap().getTileMap().at(yi).size() + xi]);
		}
	}

	////OLD CODE: Draws the whole map, which gets VERY slow at large sizes
	//for (unsigned int yi = 0; yi < game.getMap().getTileMap().size(); ++yi) {
	//	for (unsigned int xi = 0; xi < game.getMap().getTileMap().at(yi).size(); xi++) {
	//		App.draw(mapTiles[yi*game.getMap().getTileMap().at(yi).size() + xi]);
	//	}
	//}

	//Draw characters
	for (unsigned int i = 0; i < game.getMap().getCharacters().size(); ++i) {
		characterShapes[i] = sf::Sprite();
		characterShapes[i].setPosition(game.getMap().getCharacters()[i].getRenderPosition().x, game.getMap().getCharacters()[i].getRenderPosition().y);
		if (i == game.getSelectedCharacter()) {
			selectedCharacter.setPosition(game.getMap().getCharacters()[i].getRenderPosition().x, game.getMap().getCharacters()[i].getRenderPosition().y);
			App.draw(selectedCharacter);
		}
		if (game.getMap().getCharacters()[i].getTeam() == 1) {
			characterShapes[i].setTexture(texPlayer1);
			characterShapes[i].setTextureRect(sf::IntRect(game.getMap().getCharacters()[i].getDirection() * TILESIZE, game.getMap().getCharacters()[i].getAnimationFrame() % NUM_ANIMATIONS * TILESIZE, TILESIZE, TILESIZE));
		}
		else {
			characterShapes[i].setTexture(texPlayer2);
			characterShapes[i].setTextureRect(sf::IntRect(game.getMap().getCharacters()[i].getDirection() * TILESIZE, game.getMap().getCharacters()[i].getAnimationFrame() % NUM_ANIMATIONS * TILESIZE, TILESIZE, TILESIZE));
		}

		App.draw(characterShapes[i]);
	}

    App.display();
  }
}