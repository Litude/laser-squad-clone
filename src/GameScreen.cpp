#include "GameScreen.hpp"
#include "constants.hpp"
#include "Health.hpp"
#include "Ammo.hpp"
#include <SFML/Config.hpp>

#define N(T) std::make_shared<T>()

GameScreen::GameScreen(sf::RenderWindow &App, std::string mapName)
{
	m_screenResult = ScreenResult::GameScene;

	try {
		//Game logic initialization
		if (mapName.empty()) mapName = "test_level";
		game = jreader::loadJSON(mapName);
		game->setSelectedCharacter(game->getCharacters().end());
	}
	catch (JSONLoadException) {
		m_screenResult = ScreenResult::NewGameScene;
		return;
	}
	catch (JSONWriteException) {
		m_screenResult = ScreenResult::NewGameScene;
		return;
	}

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

	crosshairTexture = std::make_shared<sf::Texture>(sf::Texture());
	if (!crosshairTexture->loadFromFile("img/crosshair.png"))
	{
		std::cerr << "Error loading crosshair.png" << std::endl;
	}
	crossHairSprite.setTexture(*crosshairTexture);
	crossHairSprite.setOrigin(crossHairSprite.getGlobalBounds().width / 2, crossHairSprite.getGlobalBounds().height / 2);

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
	rayLine.setColorPoint1(sf::Color(255, 0, 0, 155));
	rayLine.setColorPoint2(sf::Color(0, 0, 255, 155));

	// Create a render-texture to draw visible tiles based on line of sight
	renderTexture_visibleTiles = std::make_shared<sf::RenderTexture>();
	if (!renderTexture_visibleTiles->create(game->getGrid().getWidth() * TILESIZE, game->getGrid().getHeight() * TILESIZE))
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

	//Status message stuff
	screenStatusMessage.setFont(*font);
	screenStatusMessage.setCharacterSize(20);

#if SFML_VERSION_MAJOR >= 2 && SFML_VERSION_MINOR >= 4
	screenStatusMessage.setOutlineThickness(2);
	screenStatusMessage.setOutlineColor(sf::Color::Black);
#endif

	// Create graphical tilemap presentation from the Map
	tileMap = std::make_shared<TileMap>(TileMap(game->getGrid()));
	if (!tileMap->load("img/tileset_grounds.png", "img/tileset_blocks.png", "img/tileset_items.png", sf::Vector2u(TILESIZE, TILESIZE))) {
		std::cout << "Could not load tilemap\n";
	}

	playerViews.resize(2);

	for (unsigned int i = 0; i < 2; ++i) {
		playerViews[i].viewCenter = gameView.getCenter(); //Fall-back in case we can't find any characters for the player
		playerViews[i].zoom = 1.0f;

		//Search for the first character owned by the player and center view on it
		for (auto it = game->getCharacters().begin(); it != game->getCharacters().end(); ++it) {
			if (it->getTeam() != i + 1) continue;
			playerViews[i].viewCenter = sf::Vector2f(it->getRenderPosition());
			break;
		}

	gameView.setCenter(playerViews[0].viewCenter);
	}
}

ScreenResult GameScreen::Run(sf::RenderWindow & App)
{
	sf::Vector2i mousePos_old = sf::Mouse::getPosition(App);
	while (App.isOpen() && m_screenResult == ScreenResult::GameScene) {
		sf::Event event;
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
			if (event.type == sf::Event::KeyPressed && game->getGameState() == GameState::active) {
				handleKeyPress(event, App);
			}
			// Handle mouse input
			if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left && game->getGameState() == GameState::active) {
				unsigned int menuSize = App.getSize().x / 4;
				if (event.mouseButton.x < static_cast<int>(App.getSize().x - menuSize)) {
					//Clicked on gamescreen
					//In shoot mode
					if (mouseMode == MouseMode::shoot && game->getSelectedCharacter() != game->getCharacters().end()) {
						auto gc = game->getSelectedCharacter();
						auto weapon = gc->getEquipped();
						auto tiles = game->characterShoot(gc, getClickedTilePosition(App));
						int k = 0;
						for (auto dest : tiles) {
							addProjectile(weapon, gc->getPosition(), dest, k*(weapon->getDelay()));
							++k;
							tileMap->updateTile(dest);
						}
						
						//std::cout << "PROJS:" << std::endl;
						for (auto proj : activeProjectiles) {
							std::cout << proj;
						}
					}
					//In select mode
					else {
						for (auto it = game->getCharacters().begin(); it != game->getCharacters().end(); ++it) {
							if (it->getTeam() == game->getCurrentPlayer() && !(it->isDead()) && it->getPosition() == getClickedTilePosition(App)) {
								game->setSelectedCharacter(it); //Select clicked character
								centerCharacter = true;
								break;
							}
							else {
								game->setSelectedCharacter(game->getCharacters().end());
							}
						}
					}
				}
			}
			if (event.type == sf::Event::Resized) {
				updateLayout(App);
			}
			if (game->getGameState() == GameState::match_ended) {
				gameOverPanel.update(event, App);
			} else {
				sidePanel.update(event, App, *game);
			}
		}

		sf::Vector2i mousePos = sf::Mouse::getPosition(App);
		float x = (float)mousePos_old.x - mousePos.x;
		float y = (float)mousePos_old.y - mousePos.y;
		mousePos_old = mousePos;
		if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right)) {
			centerCharacter = false;
			gameView.move(x, y);
		}

		// Handle time delta dependent actions
		int delta = static_cast<int>(clock.restart().asMicroseconds());
		timeAccumulator += delta;

		for (auto &character : game->getCharacters()) {
			character.update(delta);
		}
		game->getStatusMessage().updateStatusMessage(delta);
		game->removeDeadCharacters();

		App.clear();

		drawUI(App);
		drawGame(App);
		drawGameUI(App);

		App.display();
	}
	return m_screenResult;
}

void GameScreen::handleKeyPress(sf::Event& event, sf::RenderWindow& App) {
	switch (event.key.code) {
	case sf::Keyboard::A:
		if (game->getSelectedCharacter() != game->getCharacters().end() && game->getSelectedCharacter()->isMoving() == false) {
			game->characterMoveLeft(game->getSelectedCharacter());
			centerCharacter = true;
		}
		break;
	case sf::Keyboard::D:
		if (game->getSelectedCharacter() != game->getCharacters().end() && game->getSelectedCharacter()->isMoving() == false) {
			game->characterMoveRight(game->getSelectedCharacter());
			centerCharacter = true;
		}
		break;
	case sf::Keyboard::S:
		if (game->getSelectedCharacter() != game->getCharacters().end() && game->getSelectedCharacter()->isMoving() == false) {
			game->characterMoveDown(game->getSelectedCharacter());
			centerCharacter = true;
		}
		break;
	case sf::Keyboard::W:
		if (game->getSelectedCharacter() != game->getCharacters().end() && game->getSelectedCharacter()->isMoving() == false) {
			game->characterMoveUp(game->getSelectedCharacter());
			centerCharacter = true;
		}
		break;



	case sf::Keyboard::Left:
		gameView.move(-TILESIZE, 0);
		centerCharacter = false;
		break;
	case sf::Keyboard::Right:
		gameView.move(TILESIZE, 0);
		centerCharacter = false;
		break;
	case sf::Keyboard::Down:
		gameView.move(0, TILESIZE);
		centerCharacter = false;
		break;
	case sf::Keyboard::Up:
		gameView.move(0, -TILESIZE);
		centerCharacter = false;
		break;
	case sf::Keyboard::F1:
	{
		ControlsScreen helpScreen;
		helpScreen.setBackScreen(ScreenResult::GameScene);
		m_screenResult = helpScreen.Run(App);
		updateLayout(App);
		break;
	}
	case sf::Keyboard::Numpad0:
		gameView.setSize(sf::Vector2f(static_cast<float>(App.getSize().x - (App.getSize().x / 4)), static_cast<float>(App.getSize().y)));
		break;
		//enter shooting mode
	case sf::Keyboard::Q:
		toggleAttackMode();
		break;
	case sf::Keyboard::E:
		endTurn(App);
		break;
	default:
		break;
	}
}

void GameScreen::drawGame(sf::RenderWindow &App) {

	App.setView(gameView);
	// Center the camera on the selected player (linear interpolation)
	if (game->getSelectedCharacter() != game->getCharacters().end() && centerCharacter == true) {
		float factor = 0.01f;
		gameView.setCenter(sf::Vector2f(gameView.getCenter().x + (game->getSelectedCharacter()->getRenderPosition().x - gameView.getCenter().x) * factor, gameView.getCenter().y + (game->getSelectedCharacter()->getRenderPosition().y - gameView.getCenter().y) * factor));
	}

	//Draw the map
	App.draw(*tileMap);

	// Calculate visible area for all current player game characters

	if (game->calculateLineofSight()) {
		visibleTiles.clear(); //clear old calculations
		for (auto it = game->getCharacters().begin(); it != game->getCharacters().end(); ++it) {
			if (it->getTeam() != game->getCurrentPlayer() || it->isDead()) continue;
			auto newVisibleTiles = game->seenCoordinates(it);
			visibleTiles.insert(visibleTiles.end(), newVisibleTiles.begin(), newVisibleTiles.end());
		}
	}

	//Draw characters
	for (auto it = game->getCharacters().begin(); it != game->getCharacters().end(); ++it) {
		if (it->getTeam() != game->getCurrentPlayer()) {
			if (std::find(visibleTiles.begin(), visibleTiles.end(), it->getPosition()) == visibleTiles.end()) {
				// This enemy character is not visible - skip rendering
				continue;
			}
		}
		sf::Sprite characterShape;
		characterShape.setPosition(static_cast<float>(it->getRenderPosition().x), static_cast<float>(it->getRenderPosition().y));
		if (it == game->getSelectedCharacter()) {
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
		bool reached = proj->reachedDestination();
		if (proj->hasDeparted() && !reached) {
			App.draw((*proj).drawable());
		} else if (reached && !proj->hasExploded()) {
			proj->explode();
			addExplosion(*proj);
		} else if (reached && !proj->soundPlaying()) {
			// Cleanup if sound has finished playing and destination has been reached
			proj = activeProjectiles.erase(proj);
			continue;
		}
		++proj;
	}
	//Draw explosions
	for (auto expl = activeExplosions.begin(); expl != activeExplosions.end(); ) {
		if (expl->isActive()) {
			App.draw((*expl).drawable());
		} else if (!expl->soundPlaying()) {
			// Cleanup if sound has finished
			expl = activeExplosions.erase(expl);
			continue;
		}
		++expl;
	} 

	if (mouseMode == MouseMode::shoot && game->getSelectedCharacter() != game->getCharacters().end() && game->getGameState() == GameState::active) {
		auto gc = game->getSelectedCharacter();
		auto target = getClickedTilePosition(App);
		auto origin = gc->getRenderPosition();
		rayLine.setPositionPoint1(sf::Vector2f(static_cast<float>(origin.x + TILESIZE / 2), static_cast<float>(origin.y + TILESIZE / 2)));
		rayLine.setPositionPoint2(Util::mapToPixels(game->traceLineWithWeapon(gc, target)));
		//"animate" rayline
		if (rayLine.getColorPoint1().r == 255 || rayLine.getColorPoint1().r == 0) rayIncr *= -1;
		sf::Color color1 = rayLine.getColorPoint1();
		color1.r += rayIncr;
		color1.b += rayIncr * -1;
		rayLine.setColorPoint1(color1);
		sf::Color color2 = rayLine.getColorPoint2();
		color2.r += rayIncr * -1;
		color2.b += rayIncr;
		rayLine.setColorPoint2(color2);
		App.draw(rayLine);
		crossHairSprite.setPosition(rayLine.getPositionPoint2());
		crossHairSprite.setColor(color2);
		App.draw(crossHairSprite);
	}
	// Draw visible area for the selected game character
	DrawVisibleArea(App, visibleTiles);
}

// Draw visible area for the selected game character
void GameScreen::DrawVisibleArea(sf::RenderWindow &App, std::vector<sf::Vector2u> tiles) {

	if (game->calculateLineofSight()) {
		renderTexture_visibleTiles->clear(sf::Color(0, 0, 0, 0));
		for (auto it = tiles.begin(); it != tiles.end(); ++it) {
			visibleTileShape.setPosition(static_cast<float>(it->x * TILESIZE), static_cast<float>(it->y * TILESIZE));
			renderTexture_visibleTiles->draw(visibleTileShape); // or any other drawable
		}
		renderTexture_visibleTiles->display();
		game->lineofSightCalculated();
	}

	// get the target texture (where the stuff has been drawn)
	const sf::Texture& texture = renderTexture_visibleTiles->getTexture();

	// Draw visible tiles
	shader->setShaderUniform("textureSize", sf::Vector2f(texture.getSize()));
	sf::Sprite sprite(texture);
	App.draw(sprite, shader);
}

void GameScreen::drawUI(sf::RenderWindow &App) {
	updateUIComponents(App);

	//Draw elements
	App.setView(interfaceView);
	App.draw(backgroundSprite);

	if (game->getGameState() == GameState::match_ended) {
		gameOverPanel.draw(App, *game);
	}
	else {
		sidePanel.draw(App, *game, *this);
	}
}

void GameScreen::drawGameUI(sf::RenderWindow &App) {

	App.setView(interfaceView);

	screenStatusMessage.setString(game->getStatusMessage().getMessage());

	//Position must be updated every time since not all messages have the same height
	screenStatusMessage.setPosition(sf::Vector2f(-screenStatusMessage.getLocalBounds().left, App.getSize().y - (screenStatusMessage.getLocalBounds().top + screenStatusMessage.getLocalBounds().height)));

	switch (game->getStatusMessage().getSeverity()) {
	case SEVERITY_INFORMATION:
		screenStatusMessage.setTextColor(sf::Color::White);
		break;
	case SEVERITY_CRITICAL:
		screenStatusMessage.setTextColor(sf::Color::Red);
		break;
	case SEVERITY_BOOST:
		screenStatusMessage.setTextColor(sf::Color::Green);
		break;
	default:
		screenStatusMessage.setTextColor(sf::Color::White);
	}

	App.draw(screenStatusMessage);
	// Restore gameView
	App.setView(gameView);
}

void GameScreen::updateLayout(sf::RenderWindow & App)
{
	unsigned int menuSize = App.getSize().x / 4;

	/** Game View */

	gameView.setSize(static_cast<float>(App.getSize().x - menuSize), static_cast<float>(App.getSize().y));
	gameView.setViewport(sf::FloatRect(0, 0, static_cast<float>(App.getSize().x - menuSize) / static_cast<float>(App.getSize().x), 1));
	
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
	rayLine.setPositionPoint1(sf::Vector2f(0, 0));
	rayLine.setPositionPoint2(sf::Vector2f(0, 0));

	// Store view information for the current player
	playerViews.at(game->getCurrentPlayer() - 1).viewCenter = gameView.getCenter();
	playerViews.at(game->getCurrentPlayer() - 1).zoom = gameView.getSize().y / App.getSize().y;

	game->endTurn();
	
	if (game->getTurnNumber() > game->getMaxTurns()) return;

	EndTurnScreen endTurnScr;
	endTurnScr.setTurn(game->getCurrentPlayer());
	m_screenResult = endTurnScr.Run(App);
	updateLayout(App);

	// Retrieve view for the next player
	gameView.setCenter(playerViews[game->getCurrentPlayer() - 1].viewCenter);
	gameView.setSize(static_cast<float>(App.getSize().x - (App.getSize().x / 4)) * playerViews[game->getCurrentPlayer() - 1].zoom, static_cast<float>(App.getSize().y * playerViews[game->getCurrentPlayer() - 1].zoom));

}

void GameScreen::pickupItem() {
	if (game->getSelectedCharacter() != game->getCharacters().end()) {
		if (game->characterPickUpItem()) {
			tileMap->updateTile(game->getSelectedCharacter()->getPosition());
		}
	}
}

void GameScreen::dropItem() {
	if (game->getSelectedCharacter() != game->getCharacters().end()) {
		if (game->characterDropItem()) {;
			tileMap->updateTile(game->getSelectedCharacter()->getPosition());
		}
	}
}

void GameScreen::equipItem() {
	if (game->getSelectedCharacter() != game->getCharacters().end()) {
		game->characterUseItem(game->getSelectedCharacter());
	}
}

void GameScreen::toggleAttackMode() {
	mouseMode = (mouseMode == MouseMode::shoot) ? MouseMode::select : MouseMode::shoot;
	std::cout << "mousemode changed to " << mouseMode << std::endl;
}

sf::Vector2u GameScreen::getClickedTilePosition(const sf::RenderWindow& App) const {
	sf::Vector2i clickedTile = sf::Vector2i(App.mapPixelToCoords(sf::Mouse::getPosition(App), gameView));
	clickedTile.x /= TILESIZE;
	clickedTile.y /= TILESIZE;
	clickedTile.x = (clickedTile.x < 0 ? 0 : clickedTile.x);
	clickedTile.y = (clickedTile.y < 0 ? 0 : clickedTile.y);
	clickedTile.x = (clickedTile.x > static_cast<int>(game->getGrid().getWidth() - 1) ? static_cast<int>(game->getGrid().getWidth() - 1) : clickedTile.x);
	clickedTile.y = (clickedTile.y > static_cast<int>(game->getGrid().getHeight() - 1) ? static_cast<int>(game->getGrid().getHeight() - 1) : clickedTile.y);
	return sf::Vector2u(clickedTile.x, clickedTile.y);
}

void GameScreen::addProjectile(std::shared_ptr<Weapon> weapon, sf::Vector2u world_origin, sf::Vector2u world_destination, int delay) {
	AmmoType wt = weapon->getAmmoType();
	Projectile p(wt, Util::mapToPixels(world_origin), Util::mapToPixels(world_destination), delay);
	activeProjectiles.push_back(p);
}

void GameScreen::addExplosion(Projectile& proj) {
	Explosion expl(proj);
	activeExplosions.push_back(expl);
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
