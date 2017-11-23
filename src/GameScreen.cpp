#include "GameScreen.hpp"
#include "constants.hpp"

GameScreen::GameScreen(sf::RenderWindow &App)
{
	game = Game();

	game.initializeMap(30, 30);
	game.getMap().getTile(12, 12).setTile("img/tile_pavement.png", true); //Add one solid block for collision testing

	game.addCharacter(sf::Vector2u(0, 0), 1);
	game.addCharacter(sf::Vector2u(4, 4), 1);
	game.addCharacter(sf::Vector2u(8, 8), 2);
	game.addCharacter(sf::Vector2u(10, 10), 2);

	game.setSelectedCharacter(game.getCharacters().end());

	selectedCharacter = sf::RectangleShape(sf::Vector2f(TILESIZE, TILESIZE));
	selectedCharacter.setOutlineColor(sf::Color::Yellow);
	selectedCharacter.setOutlineThickness(2.0f);
	selectedCharacter.setFillColor(sf::Color::Transparent);

	texPlayer1 = std::make_shared<sf::Texture>(sf::Texture());
	if (!texPlayer1->loadFromFile("img/character1_sheet.png")) {
		std::cout << "Could not load 'img/character1_sheet.png'\n";
	}

	texPlayer2 = std::make_shared<sf::Texture>(sf::Texture());
	if (!texPlayer2->loadFromFile("img/character2_sheet.png")) {
		std::cout << "Could not load 'img/character2_sheet.png'\n";
	}

	//Initialize font
	font = std::make_shared<sf::Font>(sf::Font());
	if (!font->loadFromFile("font/ARIALN.TTF")) {
		std::cout << "Could not load 'font/ARIALN.TTF'\n";
	}

	//The size of the character or map vector should not change after initialization
	mapTiles.resize(game.getMap().getTileMap().size()*game.getMap().getTileMap().at(0).size());

	for (unsigned int i = 0; i < game.getMap().getTileMap().size(); ++i) {
		for (unsigned int j = 0; j < game.getMap().getTileMap()[i].size(); j++) {
			mapTiles[i*game.getMap().getTileMap().size() + j].setPosition(j * TILESIZE, i * TILESIZE);
			mapTiles[i*game.getMap().getTileMap().size() + j].setTexture(game.getMap().getTile(j, i).getTexture());
		}
	}
}

int GameScreen::Run(sf::RenderWindow & App)
{

	while (App.isOpen()) {
		sf::Event Event;
		while (App.pollEvent(Event)) {
			if (Event.type == sf::Event::Closed) {
				App.close();
				return -1;
			}
			//Handle keyboard input
			if (Event.type == sf::Event::KeyPressed) {
				switch (Event.key.code) {
				case sf::Keyboard::Left:
					if (game.getSelectedCharacter() != game.getCharacters().end() && game.getSelectedCharacter()->isMoving() == false) {
						game.characterMoveLeft(game.getSelectedCharacter());
					}
					break;
				case sf::Keyboard::Right:
					if (game.getSelectedCharacter() != game.getCharacters().end() && game.getSelectedCharacter()->isMoving() == false) {
						game.characterMoveRight(game.getSelectedCharacter());
					}
					break;
				case sf::Keyboard::Down:
					if (game.getSelectedCharacter() != game.getCharacters().end() && game.getSelectedCharacter()->isMoving() == false) {
						game.characterMoveDown(game.getSelectedCharacter());
					}
					break;
				case sf::Keyboard::Up:
					if (game.getSelectedCharacter() != game.getCharacters().end() && game.getSelectedCharacter()->isMoving() == false) {
						game.characterMoveUp(game.getSelectedCharacter());
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
				case sf::Keyboard::Escape:
					App.clear();
					return(0);
					break;
				}
			}
			//Handle mouse input
			if (Event.type == sf::Event::MouseButtonPressed) {

				if (Event.mouseButton.x >= App.getSize().x - MENUSIZE) {
					//Clicked on the menubar
					if (Event.mouseButton.button == 0) {
						if (Event.mouseButton.x >= App.getSize().x - MENUSIZE + 50 && Event.mouseButton.y >= 260 && Event.mouseButton.x <= App.getSize().x - 50 && Event.mouseButton.y <= 340) {
							game.endTurn();
						}
					}
				}
				else {
					//Clicked on the gamescreen

					//std::cout << App.mapPixelToCoords(sf::Mouse::getPosition(App), gameView).x << " " << App.mapPixelToCoords(sf::Mouse::getPosition(App), gameView).y << std::endl;

					// Click x and y in map tile coordinates
					unsigned int xCoord = App.mapPixelToCoords(sf::Mouse::getPosition(App), gameView).x / TILESIZE;
					unsigned int yCoord = App.mapPixelToCoords(sf::Mouse::getPosition(App), gameView).y / TILESIZE;

					//std::cout << xCoord - (xCoord % TILESIZE) << " " << yCoord - (yCoord % TILESIZE) << std::endl;
					for (auto it = game.getCharacters().begin(); it != game.getCharacters().end(); ++it) {
						if (it->getTeam() == game.getCurrentPlayer() &&
							it->getPosition().x <= xCoord &&
							it->getPosition().x + 1 >= xCoord &&
							it->getPosition().y <= yCoord &&
							it->getPosition().y + 1 >= yCoord) {
							game.setSelectedCharacter(it); //Select clicked character
							break;
						}
						else {
							game.setSelectedCharacter(game.getCharacters().end());
						}
					}
				}
			}
			if (Event.type == sf::Event::Resized) {
				resized = true;
			}
		}

		//Handle time delta dependent actions
		double delta = clock.restart().asMicroseconds();
		timeAccumulator += delta;

		for (auto &character : game.getCharacters()) {
			if (character.isMoving()) {
				character.move(delta / 1000.0f);
			}
		}

		App.clear();

		DrawUI(App);
		DrawGame(App);

		if (resized == true) resized = false;

		App.display();
	}
	return 0;
}

void GameScreen::DrawGame(sf::RenderWindow &App) {

	App.setView(gameView);

	if (resized) {
		gameView.setSize(App.getSize().x - MENUSIZE, App.getSize().y);
		gameView.setCenter((App.getSize().x - MENUSIZE) / 2, App.getSize().y / 2); //TODO: This needs to take into account changes to the view
		gameView.setViewport(sf::FloatRect(0, 0, static_cast<float>(App.getSize().x - MENUSIZE) / App.getSize().x, 1));
	}

	//Calculate which tiles we can actually see before drawing the map
	//TODO: The calculation might break when moving the view at arbitrary (non-TILESIZE) amounts
	int y0 = std::max(static_cast<int>((gameView.getCenter().y - App.getSize().y / 2) / TILESIZE), 0);
	int ymax = std::min(static_cast<int>(y0 + App.getSize().y / TILESIZE), static_cast<int>(game.getMap().getSizeY() - 1));
	int x0 = std::max(static_cast<int>((gameView.getCenter().x - (App.getSize().x - MENUSIZE) / 2) / TILESIZE), 0);
	int xmax = std::min(static_cast<int>(x0 + (App.getSize().x - MENUSIZE) / TILESIZE), static_cast<int>(game.getMap().getSizeX() - 1));

	//Draw the map
	for (unsigned int yi = y0; yi <= ymax; ++yi) {
		for (unsigned int xi = x0; xi <= xmax; xi++) {
			App.draw(mapTiles[yi*game.getMap().getTileMap().at(yi).size() + xi]);
		}
	}

	//Draw characters
	for (auto it = game.getCharacters().begin(); it != game.getCharacters().end(); ++it) {
		sf::Sprite characterShape;
		characterShape.setPosition(it->getRenderPosition().x, it->getRenderPosition().y);
		if (it == game.getSelectedCharacter()) {
			selectedCharacter.setPosition(it->getRenderPosition().x,it->getRenderPosition().y);
			App.draw(selectedCharacter);
		}
		if (it->getTeam() == 1) {
			characterShape.setTexture(*texPlayer1);
			characterShape.setTextureRect(sf::IntRect(it->getDirection() * TILESIZE, it->getAnimationFrame() % NUM_ANIMATIONS * TILESIZE, TILESIZE, TILESIZE));
		}
		else {
			characterShape.setTexture(*texPlayer2);
			characterShape.setTextureRect(sf::IntRect(it->getDirection() * TILESIZE, it->getAnimationFrame() % NUM_ANIMATIONS * TILESIZE, TILESIZE, TILESIZE));
		}

		App.draw(characterShape);
	}

}

void GameScreen::DrawUI(sf::RenderWindow &App) {

	//Interface elements that only need to be updated after resizing the window
	if (resized) {

		//Update view
		interfaceView.setSize(App.getSize().x, App.getSize().y);
		interfaceView.setCenter(App.getSize().x / 2, App.getSize().y / 2);

		//Interface background
		interfaceBkg.setSize(sf::Vector2f(MENUSIZE, App.getSize().y));
		interfaceBkg.setPosition(App.getSize().x - MENUSIZE, 0);
		interfaceBkg.setFillColor(sf::Color::Blue);

		//End turn button and text
		buttonEndTurn.setFillColor(sf::Color::Magenta);
		buttonEndTurn.setPosition(App.getSize().x - MENUSIZE + 50, 260);
		textEndTurn.setFont(*font);
		textEndTurn.setString("End turn");
		textEndTurn.setPosition(App.getSize().x - MENUSIZE + 52, 280);

		//FPS counter text
		textFPS.setFont(*font);
		textFPS.setCharacterSize(12);
		textFPS.setPosition(App.getSize().x - MENUSIZE + 52, 0);

		//Current turn text
		textCurTurn.setFont(*font);
		textCurTurn.setCharacterSize(12);
		textCurTurn.setPosition(App.getSize().x - MENUSIZE + 52, 50);

	}

	//Interface elements that always need to be updated
	textCurTurn.setString("Current turn: Player " + std::to_string(game.getCurrentPlayer()));

	//TODO: Process selected character attributes here and draw them on the interface...

	currentTime = fpsclock.getElapsedTime().asMicroseconds() / 1000000.0f;
	float fps = 1.f / (currentTime - lastTime);
	textFPS.setString("FPS: " + std::to_string(fps));
	lastTime = currentTime;

	//Draw elements
	App.setView(interfaceView);
	App.draw(interfaceBkg); //Must always be first since it covers the whole area and will hide anything "under" it
	App.draw(textFPS);
	App.draw(buttonEndTurn);
	App.draw(textCurTurn);
	App.draw(textEndTurn);

}
