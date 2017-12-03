#include "GameScreen.hpp"
#include "constants.hpp"
#include "Health.hpp"

GameScreen::GameScreen(sf::RenderWindow &App)
{
	//Game logic initialization
	game = Game();

	game.initializeGrid(30, 30);
	game.getGrid().getTile(12, 12).setTile(TileGround::dirt, wall); //Add one solid block for collision testing
	game.getGrid().getTile(12, 13).setTile(TileGround::dirt, wall); //Add one solid block for collision testing
	game.getGrid().getTile(12, 14).setTile(TileGround::dirt, wall); //Add one solid block for collision testing

	game.getGrid().getTile(5, 5).setTile(TileGround::dirt, wall); //Add one solid block for collision testing
	game.getGrid().getTile(6, 5).setTile(TileGround::dirt, wall); //Add one solid block for collision testing
	game.getGrid().getTile(7, 5).setTile(TileGround::dirt, wall); //Add one solid block for collision testing
    game.getGrid().getTile(8, 5).setTile(TileGround::dirt, wall); //Add one solid block for collision testing
    game.getGrid().getTile(9, 5).setTile(TileGround::dirt, wall); //Add one solid block for collision testing

	//game.getGrid().getTile(7, 4).setTile(TileGround::dirt, TileBlock::wall); //Add one solid block for collision testing
	//game.getGrid().getTile(7, 3).setTile(TileGround::dirt, TileBlock::wall); //Add one solid block for collision testing
	//game.getGrid().getTile(6, 3).setTile(TileGround::dirt, TileBlock::wall); //Add one solid block for collision testing

	//game.getGrid().getTile(6, 4).setTile(TileGround::dirt, TileBlock::air); //Add one solid block for collision testing
	//game.getGrid().getTile(5, 4).setTile(TileGround::dirt, TileBlock::air); //Add one solid block for collision testing

	game.getGrid().getTile(7, 2).setTile(TileGround::dirt, TileBlock::wall); //Add one solid block for collision testing

	game.getGrid().getTile(9, 7).setTile(TileGround::dirt, TileBlock::tree); //Add one solid block for collision testing
	game.getGrid().getTile(12, 15).setTile(TileGround::dirt, TileBlock::bush); //Add one solid block for collision testing

	game.getGrid().getTile(7, 4).addItem(Health_Small());

	// Test put walls on the edges of the map
	for (unsigned int i = 0; i < game.getGrid().getWidth(); ++i)
	{
		for (unsigned int j = 0; j < game.getGrid().getHeight(); ++j)
		{
			if (i == 0 || j == 0 || i == game.getGrid().getWidth() - 1 || j == game.getGrid().getHeight() - 1) {
				game.getGrid().getTile(i, j).setTile(TileGround::dirt, TileBlock::wall); //Add one solid block for collision testing
			}
		}
	}

	// Create graphical tilemap presentation from the Map
	tileMap = std::make_shared<TileMap>(TileMap(game.getGrid()));
	if (!tileMap->load("img/tileset_grounds.png", "img/tileset_blocks.png", "img/tileset_items.png", sf::Vector2u(TILESIZE, TILESIZE))) {
		std::cout << "Could not load tilemap\n";
	}

	// Test updating tile after tilemap has already been created
	game.getGrid().getTile(12, 16).setTile(TileGround::dirt, TileBlock::tree); //Add one solid block for collision testing
	game.getGrid().getTile(15, 6).addItem(Health_Large());
	tileMap->updateTile(sf::Vector2u(12, 16));
	tileMap->updateTile(sf::Vector2u(15, 6));

	game.addCharacter(sf::Vector2u(1, 1), 1);
	game.addCharacter(sf::Vector2u(4, 4), 1);
	game.addCharacter(sf::Vector2u(8, 8), 2);
	game.addCharacter(sf::Vector2u(10, 10), 2);

	game.setSelectedCharacter(game.getCharacters().end());

	//Interface drawing initialization
	font = std::make_shared<sf::Font>(sf::Font());
	if (!font->loadFromFile("font/ARIALN.TTF")) {
		std::cout << "Could not load 'font/ARIALN.TTF'\n";
	}

	//Interface element attributes that won't change during execution
	interfaceBkg.setFillColor(sf::Color::Blue);
	textFPS.setFont(*font);
	textFPS.setCharacterSize(12);
	textCurTurn.setFont(*font);
	textCurTurn.setCharacterSize(12);
	textEndTurn.setFont(*font);
	textEndTurn.setString("End turn");
	textAP.setFont(*font);
	textAP.setCharacterSize(12);
	textMouseMode.setFont(*font);
	textMouseMode.setCharacterSize(12);
	buttonEndTurn.setFillColor(sf::Color::Magenta);

	//Game drawing initialization
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

	//Ray tracing line for shooting
	rayLine[0].color = sf::Color::Red;
	rayLine[1].color = sf::Color::Green;

	// Create a render-texture to draw visible tiles based on line of sight
	renderTexture_visibleTiles = std::make_shared<sf::RenderTexture>();
	if (!renderTexture_visibleTiles->create(gameView.getSize().x, gameView.getSize().y))
	{
		std::cout << "Could not initialize render texture\n";
	}

	// Create helper shape to draw line of sight on the render texture
	visibleTileShape = sf::RectangleShape(sf::Vector2f(TILESIZE, TILESIZE));
	visibleTileShape.setOutlineColor(sf::Color::Black);
	visibleTileShape.setOutlineThickness(2.0f);
	visibleTileShape.setFillColor(sf::Color(150,150,150,255));

	// Create shader for line of sight rendering
	shader = new sf::Shader();
	if (!shader->loadFromFile("img/lineofsight_shader.frag", sf::Shader::Fragment))
	{
		std::cout << "Could not load 'img/lineofsight_shader.frag'\n";
	}
}

ScreenResult GameScreen::Run(sf::RenderWindow & App)
{	
	sf::Vector2i mousePos_old = sf::Mouse::getPosition(App);

	while (App.isOpen()) {
		sf::Event event;
		while (App.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				App.close();
				return ScreenResult::Exit;
			}
			//Handle keyboard input
			if (event.type == sf::Event::KeyPressed) {
				switch (event.key.code) {
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
					if (gameView.getCenter().x + (App.getSize().x - MENUSIZE) / 2 < game.getGrid().getWidth() * TILESIZE) gameView.move(TILESIZE, 0);
					break;
				case sf::Keyboard::S:
					if (gameView.getCenter().y + App.getSize().y / 2 < game.getGrid().getHeight() * TILESIZE) gameView.move(0, TILESIZE);
					break;
				case sf::Keyboard::W:
					if (gameView.getCenter().y - App.getSize().y / 2 > 0) gameView.move(0, -TILESIZE);
					break;
				//enter shooting mode
				case sf::Keyboard::Q:
					mouseMode = (mouseMode == MouseMode::shoot) ? MouseMode::select : MouseMode::shoot;
					std::cout << "mousemode changed to " << mouseMode << std::endl;
					break;
				default:
					break;
				}
					
			}
			//Handle mouse input
			if (event.type == sf::Event::MouseButtonReleased) {

				if (event.mouseButton.x >= App.getSize().x - MENUSIZE) {
					//Clicked on the menubar
					if (event.mouseButton.button == 0) {
						if (buttonEndTurn.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(App)))) {
							endTurn();
						}
					}
				}
				else {
					//Clicked on gamescreen
					//In shoot mode
					if (mouseMode == MouseMode::shoot and game.getSelectedCharacter() != game.getCharacters().end()) {
						game.characterShoot(game.getSelectedCharacter(), getClickedTilePosition(App, sf::Mouse::getPosition(App), gameView));
					}
					//In select mode
					else {
						for (auto it = game.getCharacters().begin(); it != game.getCharacters().end(); ++it) {
							if (it->getTeam() == game.getCurrentPlayer() && it->getPosition() == getClickedTilePosition(App, sf::Mouse::getPosition(App), gameView)) {
								game.setSelectedCharacter(it); //Select clicked character
								break;
							}
							else {
								game.setSelectedCharacter(game.getCharacters().end());
							}
						}
					}
				}
			}
			if (event.type == sf::Event::Resized) {
				resized = true;
			}
			if (event.type == sf::Event::MouseMoved and mouseMode == MouseMode::shoot and game.getSelectedCharacter() != game.getCharacters().end()) {
				auto gc = game.getSelectedCharacter();
				auto target = getClickedTilePosition(App, sf::Mouse::getPosition(App), gameView);
				auto origin = gc->getPosition();
				rayLine[0].position = Util::mapToPixels(origin);
				rayLine[1].position = Util::mapToPixels(game.traceFromCharacter(gc, target));
			}
		}

		sf::Vector2i mousePos = sf::Mouse::getPosition(App);
		float x = (float)mousePos_old.x - mousePos.x;
		float y = (float)mousePos_old.y - mousePos.y;
		mousePos_old = mousePos;
		if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
			gameView.move(x, y);
		}

		//Handle time delta dependent actions
		double delta = clock.restart().asMicroseconds();
		timeAccumulator += delta;

		for (auto &character : game.getCharacters()) {
			if (character.isMoving()) {
				character.move(delta);
			}
		}

		App.clear();

		DrawUI(App);
		DrawGame(App);

		if (resized == true) resized = false;

		App.display();
	}
	return ScreenResult::MainMenuScene;
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
	int ymax = std::min(static_cast<int>(y0 + App.getSize().y / TILESIZE) + 1, static_cast<int>(game.getGrid().getHeight() - 1));
	int x0 = std::max(static_cast<int>((gameView.getCenter().x - (App.getSize().x - MENUSIZE) / 2) / TILESIZE), 0);
	int xmax = std::min(static_cast<int>(x0 + (App.getSize().x - MENUSIZE) / TILESIZE) + 1, static_cast<int>(game.getGrid().getWidth() - 1));

	//Draw the map
	App.draw(*tileMap);

	std::vector<sf::Vector2u> visibleTiles;
	// Calculate visible area for the selected game character
	if (game.getSelectedCharacter() != game.getCharacters().end()) {
		visibleTiles = game.seenCoordinates(game.getSelectedCharacter());
	}

	//Draw characters
	for (auto it = game.getCharacters().begin(); it != game.getCharacters().end(); ++it) {
		if (it->getTeam() != game.getCurrentPlayer()) {
			if (std::find(visibleTiles.begin(), visibleTiles.end(), it->getPosition()) == visibleTiles.end()) {
				// This enemy character is not visible - skip rendering
				continue;
			}
		}
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

	if (mouseMode == MouseMode::shoot) {
		App.draw(rayLine);
	}
	// Draw visible area for the selected game character
	DrawVisibleArea(App, visibleTiles);
}

// Draw visible area for the selected game character
void GameScreen::DrawVisibleArea(sf::RenderWindow &App, std::vector<sf::Vector2u> visibleTiles) {
	// Highlight own team characters
	for (auto it = game.getCharacters().begin(); it != game.getCharacters().end(); ++it) {
		if (it->getTeam() == game.getCurrentPlayer()) {
			visibleTiles.push_back(it->getPosition());
		}
	}
	renderTexture_visibleTiles->clear(sf::Color(0, 0, 0, 0));
	for (auto it = visibleTiles.begin(); it != visibleTiles.end(); ++it) {
		visibleTileShape.setPosition(it->x * TILESIZE, it->y * TILESIZE);
		renderTexture_visibleTiles->draw(visibleTileShape); // or any other drawable
	}
	renderTexture_visibleTiles->display();

	// get the target texture (where the stuff has been drawn)
	const sf::Texture& texture = renderTexture_visibleTiles->getTexture();

	// Draw visible tiles
	sf::Sprite sprite(texture);
	App.draw(sprite, shader);
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

		//End turn button and text
		buttonEndTurn.setPosition(App.getSize().x - MENUSIZE + 50, 260);
		textEndTurn.setPosition(App.getSize().x - MENUSIZE + 52, 280);

		//FPS counter text
		textFPS.setPosition(App.getSize().x - MENUSIZE + 52, 0);

		//Current turn text
		textCurTurn.setPosition(App.getSize().x - MENUSIZE + 52, 50);

		//mousemode text
		textMouseMode.setPosition(App.getSize().x - MENUSIZE + 52, 90);

		//AP text
		textAP.setPosition(App.getSize().x - MENUSIZE + 52, 100);

	}

	//Interface elements that always need to be updated
	textCurTurn.setString("Current turn: Player " + std::to_string(game.getCurrentPlayer()));

	std::string mm = (mouseMode == MouseMode::shoot) ? "SHOOT MODE" : "SELECT MODE";
	textMouseMode.setString(mm);

	//TODO: Process selected character attributes here and draw them on the interface...
	if (game.getSelectedCharacter() != game.getCharacters().end()) {
		textAP.setString("Action points: " + std::to_string(game.getSelectedCharacter()->getActionPoints()) + '/' + std::to_string(game.getSelectedCharacter()->getMaxActionPoints()));
	}

	currentTime = fpsclock.getElapsedTime().asMicroseconds() / 1000000.0f;
	float fps = 1.f / (currentTime - lastTime);
	textFPS.setString("FPS: " + std::to_string(fps));
	lastTime = currentTime;

	//Draw elements
	App.setView(interfaceView);
	App.draw(interfaceBkg); //Must always be first since it covers the whole area and will hide anything "under" it

	// Draw action points
	if (game.getSelectedCharacter() != game.getCharacters().end()) {
		App.draw(textAP);
	}

	App.draw(textFPS);
	App.draw(textMouseMode);
	App.draw(buttonEndTurn);
	App.draw(textCurTurn);
	App.draw(textEndTurn);

}

void GameScreen::endTurn() {
	mouseMode = MouseMode::select;
	rayLine[0].position = sf::Vector2f(0, 0);
	rayLine[1].position = sf::Vector2f(0, 0);
	game.endTurn();
}

sf::Vector2u GameScreen::getClickedTilePosition(const sf::RenderWindow& App, const sf::Vector2i& point, const sf::View& view) const {
	sf::Vector2u clickedTile = sf::Vector2u(App.mapPixelToCoords(sf::Mouse::getPosition(App), gameView));
	clickedTile.x /= TILESIZE;
	clickedTile.y /= TILESIZE;
	return clickedTile;
}
