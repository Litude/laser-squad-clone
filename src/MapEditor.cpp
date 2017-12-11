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

MapEditor::MapEditor(sf::RenderWindow &App, unsigned int width, unsigned int height)
{
    m_screenResult = ScreenResult::GameScene;

	//Game logic initialization
	game = Game();

	game.initializeGrid(width, height);

	game.setSelectedCharacter(game.getCharacters().end());
    
    //Interface drawing initialization
    font = std::make_shared<sf::Font>(sf::Font());
    if (!font->loadFromFile("font/Pixellari.ttf")) {
        std::cout << "Could not load 'font/Pixellari.ttf'\n";
    }
    
    //Interface element attributes that won't change during execution
    interfaceBkg.setFillColor(sf::Color(0, 0, 0, 200));
    textFPS.setFont(*font);
    textFPS.setCharacterSize(12);
    
    backgroundTexture = std::make_shared<sf::Texture>(sf::Texture());
    if (!backgroundTexture->loadFromFile("img/background.png"))
    {
        std::cerr << "Error loading background.png" << std::endl;
    }
    backgroundSprite.setTexture(*backgroundTexture);
    
    sf::RectangleShape rs;
    rs.setFillColor(sf::Color::White);
    rs.setSize(sf::Vector2f(140, 40));
    
    buttonExit = Button("Exit to Main Menu", *font, sf::Text::Regular, 25, sf::Vector2f(0.f, 0.f), rs);
    buttonExit.setCallback([&] { this->exitToMainMenu(); });
    
    rs.setSize(sf::Vector2f(50, 20));
    
    //Game drawing initialization
    selectedTile = sf::RectangleShape(sf::Vector2f(TILESIZE, TILESIZE));
    selectedTile.setOutlineColor(sf::Color::Yellow);
    selectedTile.setOutlineThickness(2.0f);
    selectedTile.setFillColor(sf::Color::Transparent);
    selectedTile.setPosition(97,97);
    
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
    
    texItems = std::make_shared<sf::Texture>(sf::Texture());
    if (!texItems->loadFromFile("img/tileset_items.png")) {
        std::cout << "Could not load 'img/tileset_items.png'\n";
    }
   
    updateLayout(App);
    
    // Center gameview
    zoomViewAt(sf::Vector2i(gameView.getCenter().x, gameView.getCenter().y), App, gameView, 3.f);
    gameView.setCenter(sf::Vector2f(game.getGrid().getWidth() / 2 * TILESIZE, game.getGrid().getHeight() / 2 * TILESIZE));
}

ScreenResult MapEditor::Run(sf::RenderWindow & App)
{
    // Create graphical tilemap presentation from the Map
    tileMap = std::make_shared<TileMap>(TileMap(game.getGrid()));
    if (!tileMap->load("img/tileset_grounds.png", "img/tileset_blocks.png", "img/tileset_items.png", sf::Vector2u(TILESIZE, TILESIZE))) {
        std::cout << "Could not load tilemap\n";
    }
    sf::Vector2i mousePos_old = sf::Mouse::getPosition(App);
    
    
    sf::Vector2u coord=sf::Vector2u(10,10);
    selectedTile.setPosition(320,320);
    AnimationManager animManager(sf::IntRect(0, 0, 32, 32));
    
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
            // Update buttons
            buttonExit.update(event, App);
            //Handle keyboard input
            
            int xx=32;
            int yy=32;
            
            
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
            
            
            
            
            
            
            
            if (event.type == sf::Event::KeyPressed) {
                
                switch (event.key.code) {
                    case sf::Keyboard::Left:
                        if(coord.x>0){
                            coord=sf::Vector2u(coord.x-1,coord.y);
                            selectedTile.setPosition(coord.x*xx,coord.y*yy);
                        }
                        break;
                    case sf::Keyboard::Right:
                            coord=sf::Vector2u(coord.x+1,coord.y);
                            selectedTile.setPosition(coord.x*xx,coord.y*yy);
                        break;
                    case sf::Keyboard::Down:
                            coord=sf::Vector2u(coord.x,coord.y+1);
                            selectedTile.setPosition(coord.x*xx,coord.y*yy);
                        break;
                    case sf::Keyboard::Up:
                        if(coord.y>0){
                            coord=sf::Vector2u(coord.x,coord.y-1);
                            selectedTile.setPosition(coord.x*xx,coord.y*yy);
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
                        game.getGrid().getTile(coord.x, coord.y).addItem(std::make_shared<HealthPackLarge>(HealthPackLarge()));
                        tileMap->updateTile(coord);
                        break;
                    case sf::Keyboard::X:
                        game.getGrid().getTile(coord.x, coord.y).addItem(std::make_shared<HealthPackSmall>(HealthPackSmall()));
                        tileMap->updateTile(coord);
                        break;
                    case sf::Keyboard::C:
                        game.getGrid().getTile(coord.x, coord.y).addItem(std::make_shared<Pistol>(Pistol()));
                        tileMap->updateTile(coord);
                        break;
                    case sf::Keyboard::V:
                        game.getGrid().getTile(coord.x, coord.y).addItem(std::make_shared<Shotgun>(Shotgun()));
                        tileMap->updateTile(coord);
                        break;
                    case sf::Keyboard::M:
                        game.getGrid().getTile(coord.x, coord.y).addItem(std::make_shared<Ammo>(Ammo9mmBullets()));
                        tileMap->updateTile(coord);
                        break;
                    case sf::Keyboard::N:
                        game.getGrid().getTile(coord.x, coord.y).addItem(std::make_shared<Ammo>(AmmoShotgunShells()));
                        tileMap->updateTile(coord);
                        break;
                        
                        
                    case sf::Keyboard::T:
                        game.getGrid().getTile(coord.x, coord.y).setTile(TileGround::dirt, TileBlock::tree);;
                        tileMap->updateTile(coord);
                        break;
                    case sf::Keyboard::Y:
                        game.getGrid().getTile(coord.x, coord.y).setTile(TileGround::dirt, wall);
                        tileMap->updateTile(coord);
                        break;
                    case sf::Keyboard::B:
                        game.getGrid().getTile(coord.x, coord.y).setTile(TileGround::dirt, bush);
                        tileMap->updateTile(coord);
                        break;
                    case sf::Keyboard::H:
                        if(!game.addCharacter(sf::Vector2u(coord.x, coord.y), 1)){
                            std::cout << "Error" << std::endl;
                        };
                        game.setSelectedCharacter(game.getCharacters().end());
                        animManager.changeAnim(animations::walk_down);
                        for (auto &character : game.getCharacters()) {
                            character.setAnimationManager(animManager);
                        }
                        break;
                    case sf::Keyboard::J:
                        if(!game.addCharacter(sf::Vector2u(coord.x, coord.y), 2)){
                            std::cout << "Error" << std::endl;
                        };
                        game.setSelectedCharacter(game.getCharacters().end());
                        animManager.changeAnim(animations::walk_down);
                        for (auto &character : game.getCharacters()) {
                            character.setAnimationManager(animManager);
                        }
                        break;
                        
                        
                    case sf::Keyboard::R:
                        game.getGrid().getTile(coord.x, coord.y).setTile(TileGround::dirt, air);
                        game.getGrid().getTile(coord.x, coord.y).popItem();
                        game.removeCharacter(coord);
                        tileMap->updateTile(coord);
                        game.setSelectedCharacter(game.getCharacters().end());
                        break;
                    default:
                        break;
                }
                
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

void MapEditor::DrawGame(sf::RenderWindow &App) {
    
    App.setView(gameView);
    // Center the camera on the selected player (linear interpolation)
    if (game.getSelectedCharacter() != game.getCharacters().end()) {
        float factor = 0.01f;
        gameView.setCenter(sf::Vector2f(gameView.getCenter().x + (game.getSelectedCharacter()->getRenderPosition().x - gameView.getCenter().x) * factor, gameView.getCenter().y + (game.getSelectedCharacter()->getRenderPosition().y - gameView.getCenter().y) * factor));
    }
    
    //Draw the map
    App.draw(*tileMap);
    
    App.draw(selectedTile);
    
    
    //Draw characters
    for (auto it = game.getCharacters().begin(); it != game.getCharacters().end(); ++it) {
        //if (it->getTeam() != game.getCurrentPlayer()) {
            //if (std::find(visibleTiles.begin(), visibleTiles.end(), it->getPosition()) == visibleTiles.end()) {
                // This enemy character is not visible - skip rendering
                //continue;
          //  }
        //}
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
    
    //Interface elements that always need to be updated
    
    //TODO: Process selected character attributes here and draw them on the interface...
    
    currentTime = fpsclock.getElapsedTime().asMicroseconds() / 1000000.0f;
    float fps = 1.f / (currentTime - lastTime);
    textFPS.setString("FPS: " + std::to_string(fps));
    lastTime = currentTime;
    
    //Draw elements
    App.setView(interfaceView);
    App.draw(backgroundSprite);
    App.draw(interfaceBkg); //Must always be first since it covers the whole area and will hide anything "under" it
    
    App.draw(textFPS);
    App.draw(buttonExit);
}

void MapEditor::updateLayout(sf::RenderWindow & App)
{
    unsigned int menuSize = App.getSize().x / 4;
    unsigned int menuCenterX = App.getSize().x - menuSize / 2;
    unsigned int margin = 10;
    
    /** Game View */
    
    gameView.setSize(App.getSize().x - menuSize, App.getSize().y);
    gameView.setCenter((App.getSize().x - menuSize) / 2, App.getSize().y / 2); //TODO: This needs to take into account changes to the view
    gameView.setViewport(sf::FloatRect(0, 0, static_cast<float>(App.getSize().x - menuSize) / App.getSize().x, 1));
    
    /** UI View */
    
    backgroundSprite.setScale(
                              App.getSize().x / backgroundSprite.getLocalBounds().width,
                              App.getSize().y / backgroundSprite.getLocalBounds().height);
    
    interfaceView.setSize(App.getSize().x, App.getSize().y);
    interfaceView.setCenter(App.getSize().x / 2, App.getSize().y / 2);
    
    // Interface background
    interfaceBkg.setSize(sf::Vector2f(menuSize, App.getSize().y));
    interfaceBkg.setPosition(App.getSize().x - menuSize, 0);
    
    // Exit button
    buttonExit.setPosition(sf::Vector2f(menuCenterX, App.getSize().y - buttonExit.getGlobalBounds().height / 2 - margin));
    sf::RectangleShape rs;
    rs.setFillColor(sf::Color::White);
    rs.setSize(sf::Vector2f(menuSize - margin, 40));
    buttonExit.setRectangleShape(rs);
    
    updateUIComponents(App);
}

void MapEditor::updateUIComponents(sf::RenderWindow & App)
{
    unsigned int menuSize = App.getSize().x / 4;
    unsigned int menuCenterX = App.getSize().x - menuSize / 2;
    unsigned int margin = 10;
    
    // FPS counter text
    textFPS.setPosition(menuCenterX - menuSize / 2 + margin, 0);
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
