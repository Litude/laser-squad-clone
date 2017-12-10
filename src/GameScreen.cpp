#include "GameScreen.hpp"
#include "constants.hpp"
#include "Health.hpp"
#include "Ammo.hpp"

GameScreen::GameScreen(sf::RenderWindow &App)
{
	m_screenResult = ScreenResult::GameScene;

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

	game.getGrid().getTile(3, 6).addItem(std::make_shared<Ammo>(Ammo9mmBullets()));
	game.getGrid().getTile(4, 6).addItem(std::make_shared<Ammo>(Ammo9mmBullets()));
	game.getGrid().getTile(4, 7).addItem(std::make_shared<Ammo>(AmmoShotgunShells()));
	game.getGrid().getTile(4, 7).addItem(std::make_shared<Ammo>(AmmoShotgunShells()));

	game.getGrid().getTile(7, 4).addItem(std::make_shared<HealthPackSmall>(HealthPackSmall()));
	game.getGrid().getTile(7, 6).addItem(std::make_shared<Pistol>(Pistol()));
	game.getGrid().getTile(9, 6).addItem(std::make_shared<Shotgun>(Shotgun()));
	game.getGrid().getTile(15, 6).addItem(std::make_shared<HealthPackLarge>(HealthPackLarge()));
	game.getGrid().getTile(15, 6).addItem(std::make_shared<HealthPackLarge>(HealthPackLarge()));

	//Add 9 pcs to test full inventory
	game.getGrid().getTile(3, 3).addItem(std::make_shared<HealthPackLarge>(HealthPackLarge()));
	game.getGrid().getTile(3, 3).addItem(std::make_shared<HealthPackLarge>(HealthPackLarge()));
	game.getGrid().getTile(3, 3).addItem(std::make_shared<HealthPackLarge>(HealthPackLarge()));
	game.getGrid().getTile(3, 3).addItem(std::make_shared<HealthPackLarge>(HealthPackLarge()));
	game.getGrid().getTile(3, 3).addItem(std::make_shared<HealthPackLarge>(HealthPackLarge()));
	game.getGrid().getTile(3, 3).addItem(std::make_shared<HealthPackLarge>(HealthPackLarge()));
	game.getGrid().getTile(3, 3).addItem(std::make_shared<HealthPackLarge>(HealthPackLarge()));
	game.getGrid().getTile(3, 3).addItem(std::make_shared<HealthPackLarge>(HealthPackLarge()));
	game.getGrid().getTile(3, 3).addItem(std::make_shared<HealthPackLarge>(HealthPackLarge()));

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

	// Test updating tile after tilemap has already been created
	game.getGrid().getTile(12, 16).setTile(TileGround::dirt, TileBlock::tree); //Add one solid block for collision testing

	game.addCharacter(sf::Vector2u(1, 1), 1);
	game.addCharacter(sf::Vector2u(4, 4), 1);
	game.addCharacter(sf::Vector2u(8, 8), 2);
	game.addCharacter(sf::Vector2u(10, 10), 2);

	game.setSelectedCharacter(game.getCharacters().end());

	//Interface drawing initialization
	font = std::make_shared<sf::Font>(sf::Font());
	if (!font->loadFromFile("font/Pixellari.ttf")) {
		std::cout << "Could not load 'font/Pixellari.ttf'\n";
	}

	backgroundTexture = std::make_shared<sf::Texture>(sf::Texture());
	if (!backgroundTexture->loadFromFile("img/background.png"))
	{
		std::cerr << "Error loading background.png" << std::endl;
	}
	backgroundSprite.setTexture(*backgroundTexture);

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

	// Set up animations
	Animation animation_walk_left(9, 0, 8, 62000);
	Animation animation_walk_right(11, 0, 8, 62000);
	Animation animation_walk_down(10, 0, 8, 62000);
	Animation animation_walk_up(8, 0, 8, 62000);
	Animation animation_die(20, 0, 5, 125000, false);
	AnimationManager animManager(sf::IntRect(0, 0, 32, 32));
	animManager.addAnim(animation_walk_left);
	animManager.addAnim(animation_walk_right);
	animManager.addAnim(animation_walk_down);
	animManager.addAnim(animation_walk_up);
	animManager.addAnim(animation_die);
	animManager.changeAnim(animations::walk_down); // Initial animation
	for (auto &character : game.getCharacters()) {
		character.setAnimationManager(animManager);
	}

	//Ray tracing line for shooting
	rayLine[0].color = sf::Color::Red;
	rayLine[1].color = sf::Color::Blue;

	// Create a render-texture to draw visible tiles based on line of sight
	renderTexture_visibleTiles = std::make_shared<sf::RenderTexture>();
	if (!renderTexture_visibleTiles->create(game.getGrid().getWidth() * TILESIZE, game.getGrid().getHeight() * TILESIZE))
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

	sidePanel = SidePanel(App, *this);
	gameOverPanel = GameOverPanel(App, *this);

	updateLayout(App);

	healthbarBkg.setFillColor(sf::Color::Red);
	healthbarBkg.setSize(sf::Vector2f(TILESIZE, 4));
	healthbarBkg.setOutlineColor(sf::Color::Black);
	healthbarBkg.setOutlineThickness(1.0f);

	healthbar.setFillColor(sf::Color::Green);
}

ScreenResult GameScreen::Run(sf::RenderWindow & App)
{
	// Create graphical tilemap presentation from the Map
	tileMap = std::make_shared<TileMap>(TileMap(game.getGrid()));
	if (!tileMap->load("img/tileset_grounds.png", "img/tileset_blocks.png", "img/tileset_items.png", sf::Vector2u(TILESIZE, TILESIZE))) {
		std::cout << "Could not load tilemap\n";
	}
	sf::Vector2i mousePos_old = sf::Mouse::getPosition(App);
	while (App.isOpen() && m_screenResult == ScreenResult::GameScene) {
		sf::Event event;
		bool matchEnded = game.matchEnded();
		if (matchEnded) {
			game.setSelectedCharacter(game.getCharacters().end());
		}
		while (App.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				App.close();
				return ScreenResult::Exit;
			}
			if (event.type == sf::Event::MouseWheelScrolled)
			{
				if (event.mouseWheelScroll.delta > 0)
					zoomViewAt({ event.mouseWheelScroll.x, event.mouseWheelScroll.y }, App, gameView, (1.f / 1.1f));
				else if (event.mouseWheelScroll.delta < 0)
					zoomViewAt({ event.mouseWheelScroll.x, event.mouseWheelScroll.y }, App, gameView, 1.1f);
			}
			// Handle keyboard input
			if (event.type == sf::Event::KeyPressed && !matchEnded) {
				handleKeyPress(event, App);
			}
			// Handle mouse input
			if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left && !matchEnded) {
				unsigned int menuSize = App.getSize().x / 4;
				if (event.mouseButton.x < static_cast<int>(App.getSize().x - menuSize)) {
					//Clicked on gamescreen
					//In shoot mode
					if (mouseMode == MouseMode::shoot && game.getSelectedCharacter() != game.getCharacters().end()) {
						auto gc = game.getSelectedCharacter();
						auto tiles = game.characterShoot(gc, getClickedTilePosition(App, sf::Mouse::getPosition(App), gameView));
						for (auto dest : tiles) {
							addProjectile(gc->getEquipped(), gc->getPosition(), dest);
						}
						
						std::cout << "PROJS:" << std::endl;
						for (auto proj : activeProjectiles) {
							std::cout << proj;
						}
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
				updateLayout(App);
			}
			if (event.type == sf::Event::MouseMoved && mouseMode == MouseMode::shoot && game.getSelectedCharacter() != game.getCharacters().end() && !matchEnded) {
				auto gc = game.getSelectedCharacter();
				auto target = getClickedTilePosition(App, sf::Mouse::getPosition(App), gameView);
				auto origin = gc->getPosition();
				rayLine[0].position = Util::mapToPixels(origin);
				rayLine[1].position = Util::mapToPixels(game.traceFromCharacter(gc, target, true));
			}
			if (game.matchEnded()) {
				gameOverPanel.update(event, App, game);
			} else {
				sidePanel.update(event, App, game);
			}
		}

		sf::Vector2i mousePos = sf::Mouse::getPosition(App);
		float x = (float)mousePos_old.x - mousePos.x;
		float y = (float)mousePos_old.y - mousePos.y;
		mousePos_old = mousePos;
		if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right)) {
			gameView.move(x, y);
		}

		//Handle time delta dependent actions
		int delta = static_cast<int>(clock.restart().asMicroseconds());
		timeAccumulator += delta;

		for (auto &character : game.getCharacters()) {
			character.update(delta);
		}

		App.clear();

		drawUI(App);
		drawGame(App);

		App.display();
	}
	return m_screenResult;
}

void GameScreen::handleKeyPress(sf::Event& event, sf::RenderWindow& App) {
	switch (event.key.code) {
	case sf::Keyboard::Left:
		if (game.getSelectedCharacter() != game.getCharacters().end() && game.getSelectedCharacter()->isMoving() == false) {
			if (game.characterMoveLeft(game.getSelectedCharacter())) {
				//gameView.move(sf::Vector2f(-TILESIZE, 0));
			}
		}
		break;
	case sf::Keyboard::Right:
		if (game.getSelectedCharacter() != game.getCharacters().end() && game.getSelectedCharacter()->isMoving() == false) {
			if (game.characterMoveRight(game.getSelectedCharacter())) {
				//gameView.move(sf::Vector2f(TILESIZE, 0));
			}
		}
		break;
	case sf::Keyboard::Down:
		if (game.getSelectedCharacter() != game.getCharacters().end() && game.getSelectedCharacter()->isMoving() == false) {
			if (game.characterMoveDown(game.getSelectedCharacter())) {
				//gameView.move(sf::Vector2f(0, TILESIZE));
			}
		}
		break;
	case sf::Keyboard::Up:
		if (game.getSelectedCharacter() != game.getCharacters().end() && game.getSelectedCharacter()->isMoving() == false) {
			if (game.characterMoveUp(game.getSelectedCharacter())) {
				//gameView.move(sf::Vector2f(0, -TILESIZE));
			}
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

void GameScreen::drawGame(sf::RenderWindow &App) {

	App.setView(gameView);
	// Center the camera on the selected player (linear interpolation)
	if (game.getSelectedCharacter() != game.getCharacters().end()) {
		float factor = 0.01f;
		gameView.setCenter(sf::Vector2f(gameView.getCenter().x + (game.getSelectedCharacter()->getRenderPosition().x - gameView.getCenter().x) * factor, gameView.getCenter().y + (game.getSelectedCharacter()->getRenderPosition().y - gameView.getCenter().y) * factor));
	}

	//Draw the map
	App.draw(*tileMap);

	// Calculate visible area for all current player game characters

	if (game.calculateLineofSight()) {
		visibleTiles.clear(); //clear old calculations
		for (auto it = game.getCharacters().begin(); it != game.getCharacters().end(); ++it) {
			if (it->getTeam() != game.getCurrentPlayer() || it->isDead()) continue;
			auto newVisibleTiles = game.seenCoordinates(it);
			visibleTiles.insert(visibleTiles.end(), newVisibleTiles.begin(), newVisibleTiles.end());
		}
		game.lineofSightCalculated();
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
		characterShape.setPosition(static_cast<float>(it->getRenderPosition().x), static_cast<float>(it->getRenderPosition().y));
		if (it == game.getSelectedCharacter()) {
			selectedCharacter.setPosition(static_cast<float>(it->getRenderPosition().x), static_cast<float>(it->getRenderPosition().y));
			App.draw(selectedCharacter);
		}
		if (it->getTeam() == 1) {
			characterShape.setTexture(*texPlayer1);
		}
		else {
			characterShape.setTexture(*texPlayer2);
		}
		characterShape.setTextureRect(it->getAnimationManager().getFrame());

		healthbarBkg.setPosition(static_cast<float>(it->getRenderPosition().x), static_cast<float>(it->getRenderPosition().y));
		healthbar.setPosition(static_cast<float>(it->getRenderPosition().x), static_cast<float>(it->getRenderPosition().y));
		healthbar.setSize(sf::Vector2f((static_cast<float>(it->getHitpoints()) / it->getMaxHitpoints()) * TILESIZE, 4));

		App.draw(characterShape);
		App.draw(healthbarBkg);
		App.draw(healthbar);
	}

	//Draw projectiles
	for (auto proj = activeProjectiles.begin(); proj != activeProjectiles.end(); ) {
		if (proj->isActive()) {
			App.draw((*proj).drawable());
			++proj;
		} else {
			proj = activeProjectiles.erase(proj);
		}
	}

	if (mouseMode == MouseMode::shoot) {
		//"animate" rayline
		if (rayLine[0].color.r == 255 || rayLine[0].color.r == 0) rayIncr *= -1;
		rayLine[0].color.r += rayIncr;
		rayLine[0].color.b += rayIncr * -1;
		rayLine[1].color.r += rayIncr * -1;
		rayLine[1].color.b += rayIncr;
		App.draw(rayLine);
	}
	// Draw visible area for the selected game character
	DrawVisibleArea(App, visibleTiles);
}

// Draw visible area for the selected game character
void GameScreen::DrawVisibleArea(sf::RenderWindow &App, std::list<sf::Vector2u> visibleTiles) {

	renderTexture_visibleTiles->clear(sf::Color(0, 0, 0, 0));
	for (auto it = visibleTiles.begin(); it != visibleTiles.end(); ++it) {
		visibleTileShape.setPosition(static_cast<float>(it->x * TILESIZE), static_cast<float>(it->y * TILESIZE));
		renderTexture_visibleTiles->draw(visibleTileShape); // or any other drawable
	}
	renderTexture_visibleTiles->display();

	// get the target texture (where the stuff has been drawn)
	const sf::Texture& texture = renderTexture_visibleTiles->getTexture();

	// Draw visible tiles
	sf::Sprite sprite(texture);
	App.draw(sprite, shader);
}

void GameScreen::drawUI(sf::RenderWindow &App) {
	unsigned int menuSize = App.getSize().x / 4;

	updateUIComponents(App);

	//Draw elements
	App.setView(interfaceView);
	App.draw(backgroundSprite);

	if (game.matchEnded()) {
		gameOverPanel.draw(App, game, *this);
	}
	else {
		sidePanel.draw(App, game, *this);
	}
}

void GameScreen::updateLayout(sf::RenderWindow & App)
{
	unsigned int menuSize = App.getSize().x / 4;
	unsigned int menuCenterX = App.getSize().x - menuSize / 2;
	unsigned int margin = 10;

	/** Game View */

	gameView.setSize(static_cast<float>(App.getSize().x - menuSize), static_cast<float>(App.getSize().y));
	gameView.setCenter(static_cast<float>(App.getSize().x - menuSize / 2), static_cast<float>(App.getSize().y / 2));
	gameView.setViewport(sf::FloatRect(0, 0, static_cast<float>(App.getSize().x - menuSize) / static_cast<float>(App.getSize().x), 1));

	// Center the camera
	if (game.getSelectedCharacter() != game.getCharacters().end()) {
		gameView.setCenter(sf::Vector2f(static_cast<float>(game.getSelectedCharacter()->getRenderPosition().x), static_cast<float>(game.getSelectedCharacter()->getRenderPosition().y)));
	} else {
		float zoomFactor = static_cast<float>(game.getGrid().getHeight() * TILESIZE) / gameView.getSize().y;
		zoomViewAt(sf::Vector2i(static_cast<int>(gameView.getCenter().x), static_cast<int>(gameView.getCenter().y)), App, gameView, zoomFactor);
		gameView.setCenter(sf::Vector2f(static_cast<float>(game.getGrid().getWidth() / 2 * TILESIZE), static_cast<float>(game.getGrid().getHeight() / 2 * TILESIZE)));
	}
	
	/** UI View */

	backgroundSprite.setScale(
		App.getSize().x / backgroundSprite.getLocalBounds().width,
		App.getSize().y / backgroundSprite.getLocalBounds().height);

	interfaceView.setSize(static_cast<float>(App.getSize().x), static_cast<float>(App.getSize().y));
	interfaceView.setCenter(static_cast<float>(App.getSize().x / 2), static_cast<float>(App.getSize().y / 2));

	sidePanel.updateLayout(App);
	gameOverPanel.updateLayout(App);
}

void GameScreen::updateUIComponents(sf::RenderWindow & App)
{
	sidePanel.updateUIComponents(App);
	gameOverPanel.updateUIComponents(App);
}

void GameScreen::endTurn(sf::RenderWindow &App) {
	mouseMode = MouseMode::select;
	rayLine[0].position = sf::Vector2f(0, 0);
	rayLine[1].position = sf::Vector2f(0, 0);
	game.endTurn();
	// Reset view for the next player
	updateLayout(App);
}

void GameScreen::pickupItem() {
	if (game.getSelectedCharacter() != game.getCharacters().end()) {
		if (game.characterPickUpItem(game.getSelectedCharacter())) {
			tileMap->updateTile(game.getSelectedCharacter()->getPosition());
		}
	}
}

void GameScreen::dropItem() {
	if (game.getSelectedCharacter() != game.getCharacters().end()) {
		if (game.getSelectedCharacter()->getSelectedItemIndex() != -1) {
			game.characterDropItem(game.getSelectedCharacter());
			tileMap->updateTile(game.getSelectedCharacter()->getPosition());
		}
	}
}

void GameScreen::equipItem() {
	if (game.getSelectedCharacter() != game.getCharacters().end()) {
		game.getSelectedCharacter()->useSelected();
	}
}

sf::Vector2u GameScreen::getClickedTilePosition(const sf::RenderWindow& App, const sf::Vector2i& point, const sf::View& view) const {
	sf::Vector2u clickedTile = sf::Vector2u(App.mapPixelToCoords(sf::Mouse::getPosition(App), gameView));
	clickedTile.x /= TILESIZE;
	clickedTile.y /= TILESIZE;
	return clickedTile;
}

void GameScreen::addProjectile(std::shared_ptr<Weapon> weapon, sf::Vector2u world_origin, sf::Vector2u world_destination) {
	ItemIcon wt = weapon->getIcon();
	Projectile p(wt, Util::mapToPixels(world_origin), Util::mapToPixels(world_destination));
	activeProjectiles.push_back(p);
}

void GameScreen::zoomViewAt(sf::Vector2i pixel, sf::RenderWindow& window, sf::View &view, float zoom)
{
	const sf::Vector2f before{ window.mapPixelToCoords(pixel) };
	view.zoom(zoom);
	window.setView(view);
	const sf::Vector2f after{ window.mapPixelToCoords(pixel) };
	const sf::Vector2f offset{ before - after };
	view.move(offset);
	window.setView(view);
}

void GameScreen::exitToMainMenu() {
	m_screenResult = ScreenResult::MainMenuScene;
}
