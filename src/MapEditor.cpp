//
//  MapEditor.cpp
//  LaserSquad
//
//  Created by Matias Koivusalo 2 on 07/12/2017.
//
//

#include "MapEditor.hpp"
#include "constants.hpp"
#include "Health.hpp"
#include "GridLoader.hpp"

MapEditor::MapEditor(sf::RenderWindow &App, unsigned int width, unsigned int height)
{
    m_screenResult = ScreenResult::GameScene;

	//Game logic initialization
	game = Game();

	game.initializeGrid(width, height);

	game.setSelectedCharacter(game.getCharacters().end());

	if (!initComponents(App)) {
		m_screenResult = ScreenResult::Exit;
	}
}

MapEditor::MapEditor(sf::RenderWindow &App, std::string mapName)
{
	m_screenResult = ScreenResult::GameScene;

	try {
		auto game = jreader::loadJSON(mapName);

		//Game logic initialization
		game->setSelectedCharacter(game->getCharacters().end());

		if (!initComponents(App)) {
			m_screenResult = ScreenResult::Exit;
		}
	} catch (JSONLoadException) {
		m_screenResult = ScreenResult::NewMapMenuScene;
	} catch (JSONWriteException) {
		m_screenResult = ScreenResult::NewMapMenuScene;
	}
}

ScreenResult MapEditor::Run(sf::RenderWindow & App)
{
    sf::Vector2i mousePos_old = sf::Mouse::getPosition(App);
    
    AnimationManager animManager(sf::IntRect(0, 0, 32, 32));
    
    while (App.isOpen() && m_screenResult == ScreenResult::GameScene) {
        sf::Event event;
        while (App.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                App.close();
                return ScreenResult::Exit;
            }
			if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
				auto coord = getClickedTilePosition(App, sf::Vector2i(event.mouseButton.x, event.mouseButton.y), gameView);
				if (coord.x >= 0 && coord.x < game.getGrid().getWidth() && coord.y >= 0 && coord.y < game.getGrid().getHeight()) {
					selectToolCoord = coord;
				}
			}
            if (event.type == sf::Event::MouseWheelScrolled)
            {
                if (event.mouseWheelScroll.delta > 0)
                    zoomViewAt({ event.mouseWheelScroll.x, event.mouseWheelScroll.y }, App, gameView, (1.f / 1.1f));
                else if (event.mouseWheelScroll.delta < 0)
                    zoomViewAt({ event.mouseWheelScroll.x, event.mouseWheelScroll.y }, App, gameView, 1.1f);
            }
            // Update components
			sidePanel.update(event, App, game);
            //Handle keyboard input
            if (event.type == sf::Event::KeyPressed) {
				handleKeyPress(event, App);
            }
            if (event.type == sf::Event::Resized) {
                updateLayout(App);
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
        double delta = clock.restart().asMicroseconds();
        timeAccumulator += delta;
        
        for (auto &character : game.getCharacters()) {
            character.update(delta);
        }
        
        App.clear();
        
        DrawUI(App);
        DrawGame(App);
        
        App.display();
    }
    return m_screenResult;
}

void MapEditor::handleKeyPress(sf::Event& event, sf::RenderWindow& App)
{
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
	switch (event.key.code) {
	case sf::Keyboard::Left:
		if (selectToolCoord.x > 0) {
			selectToolCoord.x -= 1;
		}
		break;
	case sf::Keyboard::Right:
		if (selectToolCoord.x < game.getGrid().getWidth() - 1) {
			selectToolCoord.x += 1;
		}
		break;
	case sf::Keyboard::Down:
		if (selectToolCoord.y < game.getGrid().getHeight() - 1) {
			selectToolCoord.y += 1;
		}
		break;
	case sf::Keyboard::Up:
		if (selectToolCoord.y > 0) {
			selectToolCoord.y -= 1;
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



	case sf::Keyboard::Z:
		game.getGrid()(selectToolCoord.x, selectToolCoord.y).addItem(std::make_shared<HealthPackLarge>());
		tileMap->updateTile(selectToolCoord);
		break;
	case sf::Keyboard::X:
		game.getGrid()(selectToolCoord.x, selectToolCoord.y).addItem(std::make_shared<HealthPackSmall>());
		tileMap->updateTile(selectToolCoord);
		break;
	case sf::Keyboard::C:
		game.getGrid()(selectToolCoord.x, selectToolCoord.y).addItem(std::make_shared<Pistol>());
		tileMap->updateTile(selectToolCoord);
		break;
	case sf::Keyboard::V:
		game.getGrid()(selectToolCoord.x, selectToolCoord.y).addItem(std::make_shared<Shotgun>());
		tileMap->updateTile(selectToolCoord);
		break;
	case sf::Keyboard::M:
		game.getGrid()(selectToolCoord.x, selectToolCoord.y).addItem(std::make_shared<Ammo9mmBullets>());
		tileMap->updateTile(selectToolCoord);
		break;
	case sf::Keyboard::N:
		game.getGrid()(selectToolCoord.x, selectToolCoord.y).addItem(std::make_shared<AmmoShotgunShells>());
		tileMap->updateTile(selectToolCoord);
		break;


	case sf::Keyboard::T:
		game.getGrid()(selectToolCoord.x, selectToolCoord.y).setTile(TileGround::dirt, TileBlock::tree);;
		tileMap->updateTile(selectToolCoord);
		break;
	case sf::Keyboard::Y:
		game.getGrid()(selectToolCoord.x, selectToolCoord.y).setTile(TileGround::dirt, wall);
		tileMap->updateTile(selectToolCoord);
		break;
	case sf::Keyboard::B:
		game.getGrid()(selectToolCoord.x, selectToolCoord.y).setTile(TileGround::dirt, bush);
		tileMap->updateTile(selectToolCoord);
		break;
	case sf::Keyboard::H:
		if (!game.addCharacter(sf::Vector2u(selectToolCoord.x, selectToolCoord.y), 1)) {
			std::cout << "Error" << std::endl;
		};
		game.setSelectedCharacter(game.getCharacters().end());
		animManager.changeAnim(animations::walk_down);
		for (auto &character : game.getCharacters()) {
			character.setAnimationManager(animManager);
		}
		break;
	case sf::Keyboard::J:
		if (!game.addCharacter(sf::Vector2u(selectToolCoord.x, selectToolCoord.y), 2)) {
			std::cout << "Error" << std::endl;
		};
		game.setSelectedCharacter(game.getCharacters().end());
		animManager.changeAnim(animations::walk_down);
		for (auto &character : game.getCharacters()) {
			character.setAnimationManager(animManager);
		}
		break;


	case sf::Keyboard::R:
		game.getGrid()(selectToolCoord.x, selectToolCoord.y).setTile(TileGround::dirt, air);
		game.getGrid()(selectToolCoord.x, selectToolCoord.y).popItem();
		game.removeCharacter(selectToolCoord);
		tileMap->updateTile(selectToolCoord);
		game.setSelectedCharacter(game.getCharacters().end());
		break;
	default:
		break;
	}
}

void MapEditor::DrawGame(sf::RenderWindow &App) {
    
    App.setView(gameView);
    
    //Draw the map
    App.draw(*tileMap);
    
	selectedTile.setPosition(selectToolCoord.x*TILESIZE, selectToolCoord.y*TILESIZE);
    App.draw(selectedTile);
    
    //Draw characters
    for (auto it = game.getCharacters().begin(); it != game.getCharacters().end(); ++it) {
        sf::Sprite characterShape;
        characterShape.setPosition(it->getRenderPosition().x, it->getRenderPosition().y);
        if (it->getTeam() == 1) {
            characterShape.setTexture(*texPlayer1);
        }
        else {
            characterShape.setTexture(*texPlayer2);
        }
        characterShape.setTextureRect(it->getAnimationManager().getFrame());
        App.draw(characterShape);
    }
}

void MapEditor::DrawUI(sf::RenderWindow &App) {
    unsigned int menuSize = App.getSize().x / 4;
    
    updateUIComponents(App);
    
    //Draw elements
	App.setView(interfaceView);
	App.draw(backgroundSprite);
	sidePanel.draw(App, game, *this);
}

void MapEditor::updateLayout(sf::RenderWindow & App)
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
	}
	else {
		float zoomFactor = static_cast<float>(game.getGrid().getHeight() * TILESIZE) / gameView.getSize().y;
		zoomViewAt(sf::Vector2i(static_cast<int>(gameView.getCenter().x), static_cast<int>(gameView.getCenter().y)), App, gameView, zoomFactor);
		gameView.setCenter(sf::Vector2f(static_cast<float>(game.getGrid().getWidth() / 2 * TILESIZE), static_cast<float>(game.getGrid().getHeight() / 2 * TILESIZE)));
	}
    
    /** UI View */
    
	backgroundSprite.setScale(
		App.getSize().x / backgroundSprite.getLocalBounds().width,
		App.getSize().y / backgroundSprite.getLocalBounds().height);

	interfaceView.setSize(App.getSize().x, App.getSize().y);
	interfaceView.setCenter(App.getSize().x / 2, App.getSize().y / 2);
    
	sidePanel.updateLayout(App);
    
    updateUIComponents(App);
}

void MapEditor::updateUIComponents(sf::RenderWindow & App)
{
	sidePanel.updateUIComponents(App);
}

sf::Vector2u MapEditor::getClickedTilePosition(const sf::RenderWindow& App, const sf::Vector2i& point, const sf::View& view) const {
    sf::Vector2u clickedTile = sf::Vector2u(App.mapPixelToCoords(sf::Mouse::getPosition(App), gameView));
    clickedTile.x /= TILESIZE;
    clickedTile.y /= TILESIZE;
    return clickedTile;
}

void MapEditor::zoomViewAt(sf::Vector2i pixel, sf::RenderWindow& window, sf::View &view, float zoom)
{
    const sf::Vector2f before{ window.mapPixelToCoords(pixel) };
    view.zoom(zoom);
    window.setView(view);
    const sf::Vector2f after{ window.mapPixelToCoords(pixel) };
    const sf::Vector2f offset{ before - after };
    view.move(offset);
    window.setView(view);
}

void MapEditor::exitToMainMenu() {
    m_screenResult = ScreenResult::MainMenuScene;
}

bool MapEditor::initComponents(sf::RenderWindow & App) {
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
	selectedTile = sf::RectangleShape(sf::Vector2f(TILESIZE, TILESIZE));
	selectedTile.setOutlineColor(sf::Color::Yellow);
	selectedTile.setOutlineThickness(2.0f);
	selectedTile.setFillColor(sf::Color::Transparent);
	selectedTile.setPosition(0, 0);

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

	selectToolCoord = sf::Vector2u(0, 0);

	sidePanel = SidePanelMapEditor(App, *this);

	updateLayout(App);

	// Create graphical tilemap presentation from the Map
	tileMap = std::make_shared<TileMap>(TileMap(game.getGrid()));
	if (!tileMap->load("img/tileset_grounds.png", "img/tileset_blocks.png", "img/tileset_items.png", sf::Vector2u(TILESIZE, TILESIZE))) {
		std::cout << "Could not load tilemap\n";
	}

	return true;
}

void MapEditor::setGroundTile(TileGround tileGround) {
	auto& currentTile = game.getGrid()(selectToolCoord.x, selectToolCoord.y);
	currentTile.setTile(tileGround, currentTile.getBlock());
	tileMap->updateTile(selectToolCoord);
}

void MapEditor::setBlockTile(TileBlock tileBlock) {
	auto& currentTile = game.getGrid()(selectToolCoord.x, selectToolCoord.y);
	currentTile.setTile(currentTile.getGround(), tileBlock);
	// Update this tile and adjacent tiles in tilemap
	for (int dx = -1; dx <= 1; ++dx) {
		for (int dy = -1; dy <= 1; ++dy) {
			int coordX = selectToolCoord.x + dx;
			int coordY = selectToolCoord.y + dy;
			// Check that within bounds
			if (coordX < 0 || coordX > game.getGrid().getWidth() - 1 || coordY < 0 || coordY > game.getGrid().getHeight() - 1) {
				continue;
			}
			sf::Vector2u coord(selectToolCoord.x + dx, selectToolCoord.y + dy);
			tileMap->updateTile(coord);
		}
	}
}

void MapEditor::addItem(Item item) {
	auto& currentTile = game.getGrid().getTile(selectToolCoord.x, selectToolCoord.y);
	currentTile.addItem(std::make_shared<Item>(item));
	tileMap->updateTile(selectToolCoord);
}

void MapEditor::removeItem() {
	auto& currentTile = game.getGrid().getTile(selectToolCoord.x, selectToolCoord.y);
	currentTile.popItem();
	tileMap->updateTile(selectToolCoord);
}

void MapEditor::addCharacter(unsigned int team) {
	for (auto character : game.getCharacters()) {
		if (character.getPosition() == selectToolCoord) {
			return;
		}
	}
	Animation animation_walk_down(10, 0, 8, 62000);
	AnimationManager animManager(sf::IntRect(0, 0, 32, 32));
	animManager.addAnim(animation_walk_down);
	animManager.changeAnim(animations::walk_down);
	if (!game.addCharacter(selectToolCoord, team)) {
		std::cout << "Error" << std::endl;
	};
	for (auto &character : game.getCharacters()) {
		character.setAnimationManager(animManager);
	}
	game.setSelectedCharacter(game.getCharacters().end());
}

void MapEditor::removeCharacter() {
	game.removeCharacter(selectToolCoord);
	game.setSelectedCharacter(game.getCharacters().end());
}

bool MapEditor::saveMap(std::string name) {
	return jreader::writeJSON(game, name);
}